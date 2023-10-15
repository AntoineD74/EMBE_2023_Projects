#include "Timer_msec.h"
#include <avr/io.h>

Timer_msec::Timer_msec()
{

}

void Timer_msec::init(int period_ms)
{
    TCCR2A = 0; // set timer2 to normal operation (all bits in control register A set to zero)
    TCCR2B = 0; // set timer2 to normal operation (all bits in control register B set to zero)
    TCNT2 = 0; // initialize counter value to 0
    OCR2A = 160 / 8 - 1; // 10us
    TCCR2A |= (1 << WGM21); // clear the timer on compare match A
    TIMSK2 |= (1 << OCIE2A); // set interrupt on compare match A
    TCCR2B |= (1 << CS21); // set prescaler to 8 and start the timer

    TCCR1A = 0; // set timer1 to normal operation
    TCCR1B = 0; 
    TCNT1 = 0; // initialize counter value to 0
    OCR1A = (uint64_t{16000} / 1024)*period_ms - 1; // assign target count to compare register A 
    TCCR1B |= (1 << WGM12); // clear the timer on compare match A
    TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void Timer_msec::init(int period_ms, float duty_cycle)
{
    TCCR2A = 0; // set timer2 to normal operation (all bits in control register A set to zero)
    TCCR2B = 0; // set timer2 to normal operation (all bits in control register B set to zero)
    TCNT2 = 0; // initialize counter value to 0
    OCR2A = 160 / 8 - 1; // 10us
    TCCR2A |= (1 << WGM21); // clear the timer on compare match A
    TIMSK2 |= (1 << OCIE2A); // set interrupt on compare match A
    TCCR2B |= (1 << CS21); // set prescaler to 8 and start the timer

    TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR1B = 0; 
    TCNT1 = 0;
    OCR1A = (uint64_t{16000} / 1024)*period_ms - 1;
    OCR1B = OCR1A * duty_cycle; // assign target count to compare register B
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
    TIMSK1 |= (1 << OCIE1B); // set interrupt on compare match B
    TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer
}

void Timer_msec::set_duty_cycle(float duty_cycle)
{
    OCR1B = OCR1A * duty_cycle;
}