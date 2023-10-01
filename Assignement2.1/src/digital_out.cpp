#include <stdint.h> 
#include <avr/io.h>
#include "digital_out.h"


Digital_out::Digital_out(uint8_t portNumber)
{
    pinMask = portNumber;
}

void Digital_out::init()
{
    PORTB |= (1 << pinMask);
}

void Digital_out::set_hi()
{
    PORTB |= (1 << pinMask);
}

void Digital_out::set_lo()
{
    PORTB &= ~(1 << pinMask);
}

void Digital_out::toggle()
{
    PORTB ^= (1 << pinMask);
}

