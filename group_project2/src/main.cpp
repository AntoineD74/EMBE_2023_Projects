#include <Arduino.h>
#include "encoder.h"
#include "Analog_out.h"

bool c1_hi = false;
bool clockwise = false;

int number_us = 0;
int time_between_pulses = 0;
double speed_secpulse = 0;

double tau_speed = 1470 * 0.63;
int tau_counter;
double tau = 0;

Encoder encoder(2, 3); //c1: D2, c2: D3
Analog_out motor(2);  //pin D10

int main()
{
  Serial.begin(9600);
 
  motor.init(2000);
  motor.set(0.1);
  encoder.init();

  tau_counter = 0;

  TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
  TCCR1B = 0;
  TCNT1 = 0; 
  OCR1A = 160/8 - 1; // 10 us
  TCCR1A |= (1 << WGM12);
  TCCR1B |= (1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);
  TIMSK1 |= (1 << OCIE1B);
  TCCR1B |= (1 << CS11);

  DDRD &= ~(1 << DDD2); 
  PORTD |= (1 << PORTD2);
  EICRA |= (1 << ISC01) | (1 << ISC00); // set INT0 on rising edge
  EIMSK |= (1 << INT0);
  sei();


  while(1)
  {
    _delay_us(10);
    Serial.println(speed_secpulse);

    if(speed_secpulse == double{0})
    {
      tau_counter = 0;
    }
    
    //update speed if a new pulse is detected
    if(c1_hi)
    {
      encoder.updateCounter(clockwise);
     
      c1_hi = false;
      clockwise = false;

      speed_secpulse = double{100000}/time_between_pulses;

      if(speed_secpulse >= tau_speed && tau==0)
      {
        tau = tau_counter;
        Serial.println(tau);  // -> Multiply by 10 to get in us
      }
    }
  }

  return 0;
}

ISR (INT0_vect)
{
  c1_hi = true;
  time_between_pulses = number_us;
  tau_counter += number_us;
  number_us = 0;
  if(!encoder.is_C2_hi())
  {
    clockwise = true;
  }
}

ISR (TIMER1_COMPA_vect)
{
  number_us++;
  motor.pin.set_hi();
}

ISR(TIMER1_COMPB_vect)
{
  // led.set_lo();
  motor.pin.set_lo();
}