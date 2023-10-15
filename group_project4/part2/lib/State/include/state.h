#pragma once

#include "motor_controller.h"

class StateManager;

/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */
class State
{
    protected:
        StateManager *stateManager_;

    public:
        virtual ~State()
        {
        }

        void set_context(StateManager *context)
        {
            this->stateManager_ = context;
        }

        virtual void on_entry() = 0;
        virtual void action_looped(MotorController *motor) = 0;
        // virtual void handle_commands() = 0;
        // virtual void process_commands() = 0;

};