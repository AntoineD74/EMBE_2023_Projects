// #include "../lib/MotorController/motor_controller.h"
// #include "/lib/Encoder/encoder.h"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <time.h>


// MotorController motor(2, 1000);
// Encoder encoder(2, 3); //c1: D2, c2: D3

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

//Speed measurment
char motorInputState = 0;
int previousmotorInputState = 0;
int fd;
int speed_secpulse = 0;
int timeLastPulse = clock();
int time_between_pulses;

int period = 100000;
int duty_cyle = 0.9*100000;

int main() 
{
  // encoder.init();

  // Step 1: Export the PWM channel
int export_fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
write(export_fd, "0", 1);
close(export_fd);

// Step 2: Set the period
int period_fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
write(period_fd, "100000", 6); // Example: Set the period to 100,000 nanoseconds (0.1 ms)
close(period_fd);

// Step 3: Set the duty cycle
int duty_cycle_fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
write(duty_cycle_fd, "50000", 5); // Example: Set the duty cycle to 50,000 nanoseconds (50% duty cycle)
close(duty_cycle_fd);

// Step 4: Enable the PWM
int enable_fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
write(enable_fd, "1", 1);
close(enable_fd);

  
  while (1) 
  {

    fd = open("/dev/motor", O_RDONLY);  
    read(fd, &motorInputState, 1);
    close(fd);

    // printf("%d\n", (int)motorInputState);

    if(motorInputState != previousmotorInputState){
      // encoder.updateCounter(true);
      time_between_pulses = ((double) (clock() - timeLastPulse) * 1000000000) / CLOCKS_PER_SEC; //In ns
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

  //Unexport pwm
  fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
  write(fd, "0", 2);
  close(fd);
}
