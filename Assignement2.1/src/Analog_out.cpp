#include "Analog_out.h"


Analog_out::Analog_out(int pinNumber): pin(pinNumber), timer()
{
}

void Analog_out::init(int period_ms = 1000)
{
    timer.init(period_ms, 0.5);
    pin.init();
}

void Analog_out::set(float duty_cycle)
{
    timer.set_duty_cycle(duty_cycle);
}