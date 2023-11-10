#include <Arduino.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor_controller.h"
#include "encoder.h"

#include "StateManager.h"
#include "InitializationState.h"
#include "Operational.h"
#include "PreOperational.h"
#include "StoppedState.h"

#ifndef MODBUS_ADDRESS
#define MODBUS_ADDRESS 1
#endif

#define MSG_LEN 8

#define HIGHER_8(x) (x >> (8 * sizeof(x) - 8))
#define LOWER_8(x) (x & 0xff)
#define MAKE_16(higher, lower) (((uint16_t)higher << 8) | (uint16_t)lower)

int led = 13; // LED with PWM brightness control
uint8_t ledState = 0;

Encoder encoder(2, 3); //c1: D2, c2: D3

//speed measurement 
bool c1_hi = false;
bool clockwise = false;
uint32_t number_us = 0;
int time_between_pulses = 0;
double speed_secpulse = 0;

// Update rate
int updatePeriod = 10;  //10*10 us
int updateMsCounter = 0;
bool updatePwm = false;

//led
int ledCounter = 0;
int blinkingPeriod = 48; //48*21ms = 1008 s => 1Hz
bool changeLedState = false;

bool motorInOperation = false;

StateManager *stateManager_;

// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t buf[], int len)
{
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) { // Loop over each bit
            if ((crc & 0x0001) != 0) { // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA 001
                crc ^= 0xA001;
            }

            else // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

bool handle_crc(uint8_t msg[]){   
    uint16_t crc_arduino;
    crc_arduino = ModRTU_CRC(msg, MSG_LEN-2);
          
    /*get register count*/
    uint8_t crc_higher;
    uint8_t crc_lower;
    uint16_t crc_pi;

    crc_higher = int(msg[6]);
    crc_lower = int(msg[7]);
    crc_pi = crc_higher << 8 | crc_lower;

    return crc_arduino == crc_pi;
}

void setCrc(uint8_t* packet, size_t length) {
    uint16_t crc;

    crc = ModRTU_CRC(packet, length - 2);

    // Big endian
    packet[length - 2] = (char)HIGHER_8(crc);
    packet[length - 1] = (char)LOWER_8(crc);
}

uint8_t verifyCrc(char* packet, size_t length) {
    uint16_t crc, recv_crc;

    // Big endian
    recv_crc = MAKE_16(packet[length - 2], packet[length - 1]);
    crc = ModRTU_CRC((uint8_t*)packet, length - 2);

    if (crc != recv_crc) {
        fprintf(stderr, "Crc invalid. Expected: %02x %02x, got: %02x %02x\n",
                HIGHER_8(crc), LOWER_8(crc), HIGHER_8(recv_crc),
                LOWER_8(recv_crc));
        return -1;
    }
    return 0;
}

void read_receive(uint8_t msg[]) {
    //get registers count
    uint8_t count_higher, count_lower;
    count_higher = int(msg[4]);
    count_lower = int(msg[5]);
    uint16_t register_count = count_higher << 8 | count_lower;

    // Get register
    uint8_t register_higher, register_lower;
    register_higher = uint8_t(msg[2]);
    register_lower = uint8_t(msg[3]);
    uint16_t first_register = register_higher << 8 | register_lower;

    pinMode(PD1, OUTPUT);
    if(first_register == 1 && register_count == 1)
    {
        uint8_t packet[7] = {
            MODBUS_ADDRESS,
            uint8_t(3),
            uint8_t(register_count*2),
            uint8_t(0),
            ledState,
            0,
            0
        };
        
        setCrc(packet,sizeof(packet));

        Serial.write(packet, sizeof(packet));
    }
    else{
        //fail
        char packet[2] = {'\x83', '\x02'};
        Serial.write(packet);
    }
    pinMode(PD1, INPUT);
}

void write_receive(uint8_t msg[]) {    
    // Get value register
    uint8_t value_higher, value_lower;
    value_higher = int(msg[4]);
    value_lower = int(msg[5]);
    uint16_t register_value = value_higher << 8 | value_lower;

    // Get register
    uint8_t register_higher, register_lower;
    register_higher = uint8_t(msg[2]);
    register_lower = uint8_t(msg[3]);
    uint16_t first_register = register_higher << 8 | register_lower;

    pinMode(PD1, OUTPUT);
    if (first_register == 1){
        if (register_value == 1 || register_value == 2) {
            if(register_value == 2){ register_value = 0; }
            ledState = register_value;

            Serial.println("Changing Led state");
            digitalWrite(led, ledState);  

            uint8_t packet[MSG_LEN] = {
                MODBUS_ADDRESS,
                uint8_t(6),
                register_higher,
                register_lower,
                uint8_t(0),
                ledState,
                0,
                0
            };
            setCrc(packet,sizeof(packet));
            Serial.write(packet, sizeof(packet));
        }
        else{
            char packet[2] = {'\x86', '\x03'};
            Serial.write(packet);
        }
    }
    else 
    {
        //fail
        char packet[2] = {'\x86', '\x02'};
        Serial.write(packet);
    }
    pinMode(PD1, INPUT);
}

void setup(){
    Serial.println("Booting...");

   // baud rate of 115200 (8-bit with No parity and 1 stop bit)
    Serial.begin(115200, SERIAL_8N1);

    stateManager_ = new StateManager();
    stateManager_->receive_command('S');
    sei();
    
    // pinMode(led, OUTPUT);   
    pinMode(PD1, OUTPUT);

    digitalWrite(PD1, LOW);
    // digitalWrite(led, LOW);
}

void loop()
{
    if (Serial.available() > 0) {        
        uint8_t msg[MSG_LEN];                            
        Serial.readBytes(msg, MSG_LEN);
        
        uint8_t slave_number;
        slave_number = uint8_t(msg[0]);
        if (slave_number == MODBUS_ADDRESS) // If the command is adressed to us
        {
            Serial.println("Command received");

            if(handle_crc(msg) == true){
                uint8_t function_number;
                function_number = uint8_t(msg[1]);

                switch(function_number){

                    case 03:
                        read_receive(msg);
                        break;

                    case 06:
                        write_receive(msg);
                        break;

                    case 01:
                        stateManager_->receive_command('s');    //Set to operational state
                        blinkingPeriod = stateManager_->get_blinking_period();
                        motorInOperation = true;
                        break;

                    case 02:
                        stateManager_->receive_command('S');    //Set to stop state
                        blinkingPeriod = stateManager_->get_blinking_period();
                        motorInOperation = false;
                        break;

                    case 80:
                        stateManager_->receive_command('p');    //Set to preoperational state
                        blinkingPeriod = stateManager_->get_blinking_period();
                        motorInOperation = false;
                        break;

                    case 81:
                        stateManager_->receive_command('r');    //Reset state
                        blinkingPeriod = stateManager_->get_blinking_period();
                        motorInOperation = false;
                        break;
                    
                    case 82:
                        //Reset communication
                        break;

                    default:
                        char packet[2] = {'\x80', '\x01'};
                        Serial.write(packet);
                        break;
                }
            }
            else{
                Serial.println("Error command");
                char packet[2] = {'\x80', '\x03'};
                Serial.write(packet);
            }         
        }
    }

    if(motorInOperation){
        //update speed if a new pulse is detected
        if(c1_hi)
        {
            speed_secpulse = double{100000}/time_between_pulses;
            stateManager_->motor.currentSpeed = speed_secpulse;

            c1_hi = false;  //Turn flags down
            clockwise = false;
        }

        if(updatePwm){
            stateManager_->loopAction();
            updatePwm = false;
        }

        if(changeLedState){
            if(blinkingPeriod != 0){ stateManager_->led.toggle(); }
            changeLedState = false;
        } 
    }
}


ISR (INT0_vect)
{
  c1_hi = true;
  time_between_pulses = number_us;
  number_us = 0;
  if(!encoder.is_C2_hi())
  {
    clockwise = true;
  }
}

//Timer2: Manage update rate
ISR (TIMER2_COMPA_vect)
{
  number_us++;
  updateMsCounter++;

  if(updateMsCounter >= updatePeriod){
    updatePwm = true;
    updateMsCounter = 0;
  }
}

//Timer1: PWM + led management
ISR(TIMER1_COMPA_vect) {
  stateManager_->motor.pin.set_hi();
  ledCounter++;

  if(ledCounter == blinkingPeriod){
    changeLedState = true;
    ledCounter = 0;
  }
  else if (blinkingPeriod == 0){
    ledCounter = 0;
  }
}

ISR(TIMER1_COMPB_vect) {
  stateManager_->motor.pin.set_lo();
}

