#include "speed_controller_integrated.h"

PIController::PIController(double k, int target, double Ti, double T) : PController(k, target), ti(Ti), t(T), integral(0.0)
{
}

double PIController::updatePwm(double actual)
{
    double error;
    if(speedTarget > actual){ 
        error = speedTarget - actual;
    }
    else{ 
        error = actual - speedTarget;
    }

    integral = error*t;
    double control_signal = kp * error + kp/ti*integral;
    
    control_signal = (control_signal > 1.0 ? 0.9 : control_signal);
    return control_signal;
}

void PIController::changeParameters(double newKp, double newTi){
    kp = newKp;
    ti = newTi;
}