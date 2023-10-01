#pragma once

class Context;

/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */
class State
{
    protected:
        Context *context_;

    public:
        virtual ~State()
        {
        }

        void set_context(Context *context)
        {
            this->context_ = context;
        }

        virtual void on_entry() = 0;

        virtual void on_exit() = 0;

        virtual void on_receiving_go() = 0;

        virtual void on_receiving_stop() = 0;

};