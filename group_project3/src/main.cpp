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
int updatePeriod = 100; //updating every 100*10 us = 1000 us = 1 ms
int updateMsCounter = 0;
bool updatePwm = false;

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

//Timer2
ISR (TIMER2_COMPA_vect)
{
  number_us++;
  updateMsCounter++;
  if(updateMsCounter >= updatePeriod)
  {
    updatePwm = true;
    updateMsCounter = 0;
  }
}

//Timer1
ISR(TIMER1_COMPA_vect) {
    stateManager_->motor.pin.set_hi();
}

ISR(TIMER1_COMPB_vect) {
    stateManager_->motor.pin.set_lo();
}