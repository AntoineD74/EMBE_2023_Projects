#ifndef OPERATIONAL_STATE_H
#define OPERATIONAL_STATE_H

#include "state.h"

class OperationalState : public State {
public:
    void on_entry() override;
    void action_looped(MotorController *motor) override;
};

#endif
