#ifndef MotorController_H
#define MotorController_H

#include <timer_msec.h>
#include <digital_out.h>

class MotorController
{
    public:
        MotorController(int pinNumber);
        void init(int period_ms = 1000);
        void set(float duty_cycle);

    private:
        Timer_msec timer;

    public:    
        Digital_out pin;
};

#endif