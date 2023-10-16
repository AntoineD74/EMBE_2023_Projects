// #include "../lib/MotorController/motor_controller.h"
// #include "/lib/Encoder/encoder.h"

// #include "InitializationState.h"
// #include "Operational.h"
// #include "PreOperational.h"
// #include "StoppedState.h"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <time.h>


// MotorController motor(2, 1000);
// Encoder encoder(2, 3); //c1: D2, c2: D3

// // stateManager_ = new StateManager(new InitializationState);

// //speed measurement 
// bool c1_hi = false;
// bool clockwise = false;
// uint32_t number_us = 0;
// int time_between_pulses = 0;
// double speed_secpulse = 0;

// // Update rate
// int updatePeriod = 10;  //10*10 us
// int updateMsCounter = 0;
// bool updatePwm = false;

// //led
// int ledCounter = 0;
// int blinkingPeriod = 48; //48*21ms = 1008 s => 1Hz
// bool changeLedState = false;

//Speed measurment
char motorInputState = 0;
int previousmotorInputState = 0;
int fd;
int speed_secpulse = 0;
int timeLastPulse = clock();
int time_between_pulses;

int main() 
{
  // encoder.init();
  
  while (1) 
  {
    fd = open("/dev/motor", O_RDONLY);  
    read(fd, &motorInputState, sizeof(motorInputState));
    close(fd);

    // printf("%d\n", (int)motorInputState);

    if(motorInputState != previousmotorInputState){
      // encoder.updateCounter(true);
      // printf("HHHHHHHHHHHHHH");
      time_between_pulses = ((double) (clock() - timeLastPulse) * 1000000000) / CLOCKS_PER_SEC; //In us
      // printf("%d\n", time_between_pulses);
      
      speed_secpulse = 1000000000/time_between_pulses;
      timeLastPulse = clock();
      previousmotorInputState = motorInputState;
    }

    printf("%d\n", speed_secpulse);
    
  //   Serial.println(speed_secpulse);
  //   //update speed if a new pulse is detected
  //   if(c1_hi)
  //   {
  //     encoder.updateCounter(clockwise);
  //     speed_secpulse = double{100000}/time_between_pulses;
  //     c1_hi = false;  //Turn flags down
  //     clockwise = false;
  //   }
  //   if(updatePwm)
  //   {
  //     motor.updatePwm(speed_secpulse);  
  //     updatePwm = false;
  //   }
  }
}

// ISR (INT0_vect)
// {
//   c1_hi = true;
//   time_between_pulses = number_us;
//   number_us = 0;
//   if(!encoder.is_C2_hi())
//   {
//     clockwise = true;
//   }
// }

//Timer2: Manage update rate
// ISR (TIMER2_COMPA_vect)
// {
//   number_us++;
//   updateMsCounter++;

//   if(updateMsCounter >= updatePeriod){
//     updatePwm = true;
//     updateMsCounter = 0;
//   }
// }

// //Timer1: PWM + led management
// ISR(TIMER1_COMPA_vect) {
//   Serial.println("Time1");
  // stateManager_->motor.pin.set_hi();
  // ledCounter++;

  // if(ledCounter == blinkingPeriod){
  //   changeLedState = true;
  //   ledCounter = 0;
  // }
  // else if (blinkingPeriod == 0){
  //   ledCounter = 0;
  // }
// }

// ISR(TIMER1_COMPB_vect) {
//   // stateManager_->motor.pin.set_lo();
//   Serial.println("Time2");
// }