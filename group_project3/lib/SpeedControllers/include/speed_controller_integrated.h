#ifndef PICONTROLLER_H
#define PICONTROLLER_H

#include "speed_controller.h"

class PIController : public PController
{
    public:
        PIController(double k, int target, double Ti, double T);
        double updatePwm(double actual);

    private:
        double ti;
        double t;
        double integral;

};

#endif