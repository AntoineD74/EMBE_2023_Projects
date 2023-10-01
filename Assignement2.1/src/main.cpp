#include <digital_out.h>
#include <timer_msec.h>

#include "avr/io.h"

#include <avr/interrupt.h>

#include "Analog_out.h"

// Digital_out led(5);
// Timer_msec timer;
Analog_out timerLed(2);


//Part 1 and 2
// int main() 
// {
//     led.init();
//     timer.init(500);
//     // timer.init(21, 0.2);
//     sei(); // Enable global interrupts
//     // timer.set_duty_cycle(0.9);   //Testing the set_duty_cylcle method


//     while (1) {
//         // Your main program loop
//     }
// }

// Part 3
int main() 
{
    timerLed.init(21);
    timerLed.set(0.9);  //Change this value to control the intensity of the led
    sei();

    while (1) {
    }
}

ISR(TIMER1_COMPA_vect) {
    // led.toggle();    //part 1
    // led.set_hi();    //part 2
    timerLed.pin.set_hi();  //part 3
}

ISR(TIMER1_COMPB_vect) {
    // led.set_lo();
    timerLed.pin.set_lo();
}