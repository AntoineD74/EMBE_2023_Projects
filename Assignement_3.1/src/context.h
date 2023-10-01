#include "state.h"

/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */
class Context
{

    private:
        State *state_;

    public:
        Context(State *state) : state_(nullptr)
        {
            this->transition_to(state);
        }

        ~Context()
        {
            delete state_;
        }

    /**
     * The Context allows changing the State object at runtime.
     */

    void transition_to(State *state)
    {
        if (this->state_ != nullptr)
        {
        this->state_->on_exit();
        delete this->state_;
        }

        this->state_ = state;

        this->state_->set_context(this);

        this->state_->on_entry();
    }

    /**
     * The Context delegates part of its behavior to the current State object.
     */


    void on_receiving_go()
    {
        this->state_->on_receiving_go();
    }

    void on_receiving_stop()
    {
        this->state_->on_receiving_stop();
    }
};