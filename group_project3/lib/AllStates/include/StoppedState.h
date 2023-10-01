 #ifndef STOPPED_STATE_H
#define STOPPED_STATE_H

#include "state.h"

class StoppedState : public State {
public:
    void on_entry() override;
    void handle_commands() override;
    void process_commands() override;
};

#endif
