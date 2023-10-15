#include <avr/io.h>
#include "encoder.h"

Encoder::Encoder(int pin1, int pin2): pinC1(pin1), pinC2(pin2), position(0)
{
    
}

 void Encoder::init()
 {
    PORTD &= ~(1 << pinC1);
    PORTD |= (1 << pinC1);  //enable pull-up resistor
    PORTD &= ~(1 << pinC2);
    PORTD |= (1 << pinC2);
 }

bool Encoder::is_C1_hi()
{
    return PIND & (1 << pinC1);
}

bool Encoder::is_C2_hi()
{
    return PIND & (1 << pinC2);
}

void Encoder::updateCounter(bool clockwise)
{
    if(clockwise){ position++; }
    else{ position--; }
}

int Encoder::getPosition()
{
    return position;
}

