#ifndef MotorController_H
#define MotorController_H

#include <timer_msec.h>
#include <digital_out.h>

class MotorController
{
    public:
        MotorController(int pinNumber, int target);
        void init(int period_ms = 1000);
        void set(float duty_cycle);
        void updatePwm();
        void brake();
        int currentSpeed;

    private:
        Timer_msec timer;
        int speedTarget;
        double kp;

    public:    
        Digital_out pin;
};

#endif