#include "speed_controller.h"


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