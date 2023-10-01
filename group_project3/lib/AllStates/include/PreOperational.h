#ifndef PRE_OPERATIONAL_STATE_H
#define PRE_OPERATIONAL_STATE_H

#include "state.h"

class PreOperationalState : public State {
public:
    void on_entry() override;
    void handle_commands() override;
    void process_commands() override;
};

#endif