#ifndef MotorController_H
#define MotorController_H

#include <timer_msec.h>
#include <digital_out.h>
#include "speed_controller.h"
#include "speed_controller_integrated.h"

class MotorController
{
    public:
        MotorController(int pinNumber, int target);
        void init(int period_ms = 1000);
        void set(float duty_cycle);
        void updatePwm();
        void brake();
        int currentSpeed;
        Digital_out pin;
        PIController speedController;
        PController speedController2;
        bool useIntegrated;
    
    private:
        Timer_msec timer;
        // PController speedController;
        
        
        
};

#endif