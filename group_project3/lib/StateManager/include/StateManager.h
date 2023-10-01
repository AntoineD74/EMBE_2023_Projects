#ifndef MY_STATE_MACHINE_H
#define MY_STATE_MACHINE_H

// #include <Arduino.h>
#include "motor_controller.h"
#include "state.h"


#include "StateManager.h"
#include "InitializationState.h"
#include "Operational.h"
#include "PreOperational.h"
#include "StoppedState.h"

class StateManager
{
    private:
        State *state_;
        int currentStateIndex;

    public:
        StateManager();
        ~StateManager();
        void loopAction();
        void transitionToState(State *state);
        void receive_command(char cmd);
        MotorController motor;
};

#endif















