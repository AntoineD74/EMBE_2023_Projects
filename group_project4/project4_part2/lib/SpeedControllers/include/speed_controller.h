#ifndef PCONTROLLER_H
#define PCONTROLLER_H

#include "speed_controller.h"

class PController
{
    public:
        int speedTarget;
        PController(double k, int target);
        double updatePwm(double actual);
        void changeParameters(double newKp);
        double kp;
};

PController::PController(double k, int target): kp(k), speedTarget(target)
{

}

double PController::updatePwm(double actual)
{
    double error;
    if(speedTarget > actual){ 
        error = speedTarget - actual;
    } else{ 
        error = actual - speedTarget;
    }

    return (kp * error);
}

void PController::changeParameters(double newKp){
    kp = newKp;
}

#endif