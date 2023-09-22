#include "speed_controller.h"

PController::PController(double k): kp(k)
{
}

double PController::update(double ref , double actual)
{
    double error;
    if(ref > actual){ error = ref - actual;}
    else{ error = actual - ref;}
    double control_signal = kp * error;
    return control_signal;
}