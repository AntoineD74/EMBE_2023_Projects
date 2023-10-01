#ifndef MY_STATE_MACHINE_H
#define MY_STATE_MACHINE_H

#include <Arduino.h>

#include "state.h"


class StateManager
{

    private:
        State *state_;

    public:
        StateManager(State *state) : state_(nullptr)
        {
            this->transition_to(state);
        }

        ~StateManager()
        {
            delete state_;
        }
        

        // The state manager allows changing the State object at runtime.
        void transition_to(State *state)
        {
            if (this->state_ != nullptr)
            {
                // this->state_->on_exit();
                delete this->state_;
            }

            this->state_ = state;

            this->state_->set_context(this);

            this->state_->on_entry();
        }

        // Process the command received to transition to a different state
        void receive_command(char cmd)
        {
            // this->state_->process_commands(cmd);
        }
};


// class MyStateMachine {
// public:
// MyStateMachine();
// ~MyStateMachine(); 
// void setup(); 
// void loop(); 
// void on_receiving_go();
// void on_receiving_stop();
// void sendBootMessage();

// private:
// State* currentState; 
// State* nextState
// void transitionToState(State* nextState);

// };

#endif















