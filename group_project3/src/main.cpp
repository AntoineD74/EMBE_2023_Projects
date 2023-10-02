#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "motor_controller.h"
#include "encoder.h"

#include "StateManager.h"
#include "InitializationState.h"
#include "Operational.h"
#include "PreOperational.h"
#include "StoppedState.h"

// MotorController motor(2, 1000);
Encoder encoder(2, 3); //c1: D2, c2: D3

// stateManager_ = new StateManager(new InitializationState);

//speed measurement 
bool c1_hi = false;
bool clockwise = false;
uint32_t number_us = 0;
int time_between_pulses = 0;
double speed_secpulse = 0;

// Update rate
int updatePeriod = 1; //updating every 100*10 us = 1000 us = 1 ms
int updateMsCounter = 0;
bool updatePwm = false;

//led
int ledCounter = 0;
int blinkingPeriod = 48; //48*21ms = 1008 s => 1Hz
bool changeLedState = false;

//pwm
// float pwm = 0.9;
// int pwm_counter = 0;

/*
int main() 
{
  Serial.begin(9600);

  motor.init(21);
  motor.set(pwm);  //Change this value to control the intensity of the led
  encoder.init();

  sei();

  while (1) 
  {
    Serial.println(speed_secpulse);

    //update speed if a new pulse is detected
    if(c1_hi)
    {
      encoder.updateCounter(clockwise);
      speed_secpulse = double{100000}/time_between_pulses;
      c1_hi = false;  //Turn flags down
      clockwise = false;
    }

    if(updatePwm)
    {
      motor.updatePwm(speed_secpulse);  
      updatePwm = false;
    }
  }
}

*/
//

StateManager *stateManager_;

int main()
{
  Serial.begin(9600); 

  stateManager_ = new StateManager();
  
  sei();

  while (true)
  {
    int command = 0;
    if (Serial.available() > 0)
    {
      command = Serial.read();
      stateManager_->receive_command(command);
      blinkingPeriod = stateManager_->get_blinking_period();
    }

    //update speed if a new pulse is detected
    if(c1_hi)
    {
      speed_secpulse = double{100000}/time_between_pulses;
      stateManager_->motor.currentSpeed = speed_secpulse;

      c1_hi = false;  //Turn flags down
      clockwise = false;
    }

    if(updatePwm)
    {
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