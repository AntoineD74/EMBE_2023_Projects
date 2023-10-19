#ifndef PICONTROLLER_H
#define PICONTROLLER_H

#include "speed_controller.h"
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

class PIController : public PController
{
    public:
        PIController(double k, int target, double Ti, double T);
        double updatePwm(double actual);
        void changeParameters(double newKp, double newTi);

    private:
        double ti;
        double t;
        double integral;
};


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

#endif