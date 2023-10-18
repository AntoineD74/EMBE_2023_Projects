#include "lib/Encoder/include/encoder.h"
#include "lib/SpeedControllers/include/speed_controller_integrated.h"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <time.h>
#include <string>

using namespace std;

int speedTarget = 500;
PIController speedController(0.001, speedTarget, 0.021, 0.02);
Encoder encoder;

// Update rate
int updatePeriod = 100;
int updateCounter = 0;
double lastUpdate = clock();

//Speed measurment
char motorInputState = 0;
int previousmotorInputState = 0;
int fd;
int speed_secpulse = 0;
int timeLastPulse = clock();
int time_between_pulses;

//Pwm management
double pwm = 0.9;
int period = 100000;
int duty_cycle = pwm*period;
string convertedString;

int main() 
{  
  int export_fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
  write(export_fd, "0", 1);
  close(export_fd);

  int period_fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
  convertedString = to_string(period);
  write(period_fd, convertedString.c_str(), convertedString.length()); // Example: Set the period to 100,000 nanoseconds (0.1 ms)
  close(period_fd);

  int duty_cycle_fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
  convertedString = to_string(duty_cycle);
  write(duty_cycle_fd, convertedString.c_str(), convertedString.length()); // Example: Set the duty cycle to 50,000 nanoseconds (50% duty cycle)
  close(duty_cycle_fd);

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
      encoder.updateCounter(1);
      time_between_pulses = ((double) (clock() - timeLastPulse) * 1000000000) / CLOCKS_PER_SEC; //In ns
      // printf("%d\n", time_between_pulses);
      speed_secpulse = 1000000000/time_between_pulses;
      timeLastPulse = clock();
      previousmotorInputState = motorInputState;
    }

    printf("Instantaneous speed: %d\n", speed_secpulse);

    if((clock() - lastUpdate) * 1000 / CLOCKS_PER_SEC){ //In ms
      updateCounter++;
      
      if(updateCounter == updatePeriod){
        pwm = speedController.updatePwm((double) speed_secpulse);
        // printf("PWM: %d\n", pwm);
        duty_cycle = pwm*period;  // Recompute ducty cycle
        // int duty_cycle_fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
        // convertedString = to_string(duty_cycle);
        // write(duty_cycle_fd, convertedString.c_str(), convertedString.length()); // Example: Set the duty cycle to 50,000 nanoseconds (50% duty cycle)
        // close(duty_cycle_fd);
        
        updateCounter = 0;
      }
    }
  }

  //Unexport pwm
  fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
  write(fd, "0", 2);
  close(fd);
}
