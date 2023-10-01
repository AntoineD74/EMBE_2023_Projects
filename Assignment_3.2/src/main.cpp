#include <Arduino.h>
#include <digital_out.h>
#include <fifo.h>
#include <fifoCircular.h>

bool popOffChar = false;

int main()
{
  Serial.begin(9600);

  Digital_out led(5);
  // Fifo fifoBuffer;
  FifoCircular fifoBuffer;

  led.init();
  
  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1 = 0; 
  OCR1A = 16000000 / 1024 - 1; // 1s timer
  TCCR1B |= (1 << WGM12); 
  TIMSK1 |= (1 << OCIE1A); 
  TCCR1B |= (1 << CS12) | (1 << CS10);
  sei();

  while(1)
  {
    int command = 0;
    if (Serial.available() > 0)
    {
      led.set_hi();
      command = Serial.read();
      fifoBuffer.put(command);
      led.set_lo();
    }

    if(popOffChar)
    {
      if(!fifoBuffer.is_empty()){ 
        Serial.println((char) fifoBuffer.get()); 
      }
      else{ 
        Serial.println("Buffer Empty !"); 
      }
      popOffChar = false;
    }
  }

}

ISR(TIMER1_COMPA_vect)
{
  popOffChar = true;
}
