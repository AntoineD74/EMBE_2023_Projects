#include <avr/io.h>
#include <digital_in.h>

Digital_in::Digital_in(uint8_t pinNumber): pinMask(pinNumber){}

void Digital_in::init()
{
    PORTB &= ~(1 << pinMask);
}

bool Digital_in::is_hi()
{
    return PINB & (1 << pinMask);
}

bool Digital_in::is_lo()
{
    return !is_hi();
}