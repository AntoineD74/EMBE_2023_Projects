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

/* ==================== Available Commands =========================
  s : set -> go to operational state
  S : Stop -> go to operational state
  r : reset -> go to operational state
  p : preoperation -> go to operational state
====================================================================*/

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
int updatePeriod = 10;  //10*10 us
int updateMsCounter = 0;
bool updatePwm = false;

//led
int ledCounter = 0;
int blinkingPeriod = 48; //48*21ms = 1008 s => 1Hz
bool changeLedState = false;

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