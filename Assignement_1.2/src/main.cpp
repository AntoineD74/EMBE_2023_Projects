#include <avr/io.h>
#include <util/delay.h>
#include <digital_out.h>
#include <digital_in.h>

const uint8_t LED_PIN = (1 << PB5);

int main()
{
  //------------------------------------ PART 1 ------------------------------------//

  // int half_period = (int) (((1/1.5)/2)*1000);  //calculating period in ms
  // PINB |= LED_PIN; //Define PB5 as output

  // while(1)
  // {
  //   //Test Part 1
  //   _delay_ms ( half_period );
  //   PORTB |= LED_PIN;
  //   _delay_ms ( half_period );
  //   PORTB &= ~LED_PIN;
  // }

  //------------------------------------ PART 2 ------------------------------------//

  // Digital_out led(5);   // PB5 Arduino Nano built-in LED 
  
  // led.init();

  // while (1)
  // {
  //   _delay_ms(1000);
  //   led.toggle();
  // }

  //------------------------------------ PART 3 ------------------------------------//

  Digital_out led(1); //Built-in LED
  Digital_in button(2); //Pin D10

  led.init();
  button.init();

  while (1)
  {

    if (button.is_hi())
    {
      led.toggle();
    }
  }

  return 0;
}