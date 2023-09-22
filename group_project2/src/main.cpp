#include <stdint.h>
#include <Arduino.h>
#include "encoder.h"
#include "Digital_out.h"

/*-----------------------------------------------Part 3----------------------------------------------*/

// bool c1_hi = false;
// bool clockwise = false;

// uint32_t number_us = 0;
// int time_between_pulses = 0;
// double speed_secpulse = 0;

// double tau_speed = 1470 * 0.63;
// int tau_counter;
// double tau = 0;

// int pwm_counter = 0;
// int pmw_full_period = 10; //full period pwm = 640 us
// int pmw_off_time = (1-0.4) * pmw_full_period;
// bool pwm_current_state = false;


// Encoder encoder(2, 3); //c1: D2, c2: D3
// Digital_out motor(2);  //pin D10

// int main()
// {
//   Serial.begin(9600);
 
//   motor.init();
//   encoder.init();

//   tau_counter = 0;

//   TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
//   TCCR1B = 0;
//   TCNT1 = 0; // initialize counter value to 0
//   OCR1A = 160/8 - 1; // 10us
//   TCCR1B |= (1 << WGM12); // clear the timer on compare match A
//   TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
//   TCCR1B |= (1 << CS11); // set prescaler to 8 and start the time

//   TCCR2A = 0;
//   TCCR2B = 0;
//   TCNT2 = 0;
//   OCR2A = 16000000/1024 - 1; //64us
//   TCCR2A |= (1 << WGM12); // Configure Timer 2 for CTC mode
//   TIMSK2 |= (1 << OCIE2A); // Enable the Timer 2 compare match A interrupt
//   TCCR2B |= (1 << CS12) | (1 << CS10);

//   DDRD &= ~(1 << DDD2); 
//   PORTD |= (1 << PORTD2);
//   EICRA |= (1 << ISC01) | (1 << ISC00); // set INT0 on rising edge
//   EIMSK |= (1 << INT0);
//   sei();


//   while(1)
//   {
//     Serial.println(speed_secpulse);

//     if(speed_secpulse == double{0})
//     {
//       tau_counter = 0;
//     }
    
//     //update speed if a new pulse is detected
//     if(c1_hi)
//     {
//       encoder.updateCounter(clockwise);
//       speed_secpulse = double{100000}/time_between_pulses;

//       if(speed_secpulse >= tau_speed && tau==0)
//       {
//         tau = tau_counter;
//         // Serial.println(tau);  // -> Multiply by 10 to get in us
//       }

//       c1_hi = false;  //Turn flags down
//       clockwise = false;
//     }
//   }

//   return 0;
// }

// ISR (INT0_vect)
// {
//   c1_hi = true;
//   time_between_pulses = number_us;
//   tau_counter += number_us;
//   number_us = 0;
//   if(!encoder.is_C2_hi())
//   {
//     clockwise = true;
//   }
// }

// ISR (TIMER1_COMPA_vect)
// {
//   number_us++;
// }

// ISR(TIMER2_COMPA_vect)
// {
//   pwm_counter++;
  
//   if(pwm_counter == (pmw_full_period-pmw_off_time))
//   {
//     motor.set_lo(); //end of the cycle
//   }
  
//   else if (pwm_counter == pmw_full_period)
//   {
//     pwm_counter = 0;
//     motor.set_hi(); //start the cycle again
//   }
// }

/*-----------------------------------------------Part 4----------------------------------------------*/
#include "speed_controller.h"

int targetSpeed = 900;

bool c1_hi = false;
bool clockwise = false;

//speed measurement 
uint32_t number_us = 0;
int time_between_pulses = 0;
double speed_secpulse = 0;

// Update rate
int updatePeriod = 100; //updating every 100*10 us = 1000 us = 1 ms
int updateMsCounter = 0;
bool updatePwm = false;

//tau measurement
double tau_speed = 1470 * 0.63;
int tau_counter;
double tau = 0;

//pwm
float pwm = 0.9;
int pwm_counter = 0;
int pmw_full_period = 50; //full period pwm = 50*64 us
int pmw_off_time = (1 - pwm) * pmw_full_period;
bool pwm_current_state = false;

Encoder encoder(2, 3); //c1: D2, c2: D3
Digital_out motor(2);  //pin D10
PController speedController(0.001);

int main()
{
  Serial.begin(9600);
 
  motor.init();
  encoder.init();

  tau_counter = 0;

  TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
  TCCR1B = 0;
  TCNT1 = 0; // initialize counter value to 0
  OCR1A = 160/8 - 1; // 10us
  TCCR1B |= (1 << WGM12); // clear the timer on compare match A
  TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
  TCCR1B |= (1 << CS11); // set prescaler to 8 and start the time

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 16000000/1024 - 1; //64us
  TCCR2A |= (1 << WGM12); // Configure Timer 2 for CTC mode
  TIMSK2 |= (1 << OCIE2A); // Enable the Timer 2 compare match A interrupt
  TCCR2B |= (1 << CS12) | (1 << CS10);

  DDRD &= ~(1 << DDD2); 
  PORTD |= (1 << PORTD2);
  EICRA |= (1 << ISC01) | (1 << ISC00); // set INT0 on rising edge
  EIMSK |= (1 << INT0);
  sei();


  while(1)
  {
    Serial.print(speed_secpulse);
    Serial.print("\\");
    Serial.println(pwm);

    if(updatePwm)
    {
      pwm = speedController.update(targetSpeed , speed_secpulse);  
      pmw_off_time = (1 - pwm) * pmw_full_period; // Recompute pwm off period
      updatePwm = false;
    }

    if(speed_secpulse == double{0})
    {
      tau_counter = 0;
    }
    
    //update speed if a new pulse is detected
    if(c1_hi)
    {
      encoder.updateCounter(clockwise);
      speed_secpulse = double{100000}/time_between_pulses;

      if(speed_secpulse >= tau_speed && tau==0)
      {
        tau = tau_counter;
        // Serial.println(tau);  // -> Multiply by 10 to get in us
      }

      c1_hi = false;  //Turn flags down
      clockwise = false;
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
  updateMsCounter++;
  if(updateMsCounter >= updatePeriod)
  {
    updatePwm = true;
    updateMsCounter = 0;
  }
}

ISR(TIMER2_COMPA_vect)
{
  pwm_counter++;
  
  if(pwm_counter == (pmw_full_period-pmw_off_time))
  {
    motor.set_lo(); //end of the cycle
  }
  
  else if (pwm_counter == pmw_full_period)
  {
    pwm_counter = 0;
    motor.set_hi(); //start the cycle again
  }
}