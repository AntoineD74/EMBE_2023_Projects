#ifndef INITIALIZATION_STATE_H
#define INITIALIZATION_STATE_H

#include "state.h"

class InitializationState : public State {
public:
    void on_entry() override;
    void handle_commands() override;
    void process_commands() override;
};

#endif