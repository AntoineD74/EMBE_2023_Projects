#ifndef PCONTROLLER_H
#define PCONTROLLER_H

#include "speed_controller.h"

class PController
{
    public:
        int speedTarget;
        PController(double k, int target);
        double updatePwm(double actual);

    private:
        double kp;
};

#endif