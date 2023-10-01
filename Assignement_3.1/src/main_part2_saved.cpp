// #include <Arduino.h>

// int command = 0; // for incoming serial data
// bool timerChangeState = false;
// int secondCounter;

// int main()
// {

//   Serial.begin(9600); 
  
//   TCCR1A = 0; 
//   TCCR1B = 0; 
//   TCNT1 = 0; 
//   OCR1A = 16000000 / 1024 - 1; // 1s timer
//   TCCR1B |= (1 << WGM12); 
//   TIMSK1 |= (1 << OCIE1A); 
//   TCCR1B |= (1 << CS12) | (1 << CS10);
//   sei();
  
//   enum State
//   {
//     STATE_A,
//     STATE_B,
//     STATE_C
//   };

//   State state = State::STATE_A; // Define state variable and set the initial state

//   bool goCommand = false;
//   bool waitingMessageDisplayed = false;

//   while (true)
//   {
//     // poll or wait for events
//     if (Serial.available() > 0)
//     {
//       // read the incoming byte:
//       command = Serial.read();
      
//       // Serial.print("I received: "); // print the command received
//       // Serial.println(command, DEC);

//       switch (command)
//       {
//         case 'g':
//           Serial.print("I received a go command");
//           goCommand = true;
//           break;

//         case 's':
//           Serial.print("I received a stop command");
//           goCommand = false;
//           break;
//       }
//     }

//     switch (state)
//     {
//       case State::STATE_A:
//         if (timerChangeState && goCommand)
//         {
//           state = State::STATE_B;
//           Serial.println("\n[+]State change: switching to state B");
//           waitingMessageDisplayed = false;
//           timerChangeState = false;
//         }
//         else if (timerChangeState && !goCommand)
//         {
//           if(!waitingMessageDisplayed)
//           {
//             Serial.println("\nState change paused: waiting to receive go command...");
//             waitingMessageDisplayed = true;
//           }
//         }
//         break;

//       case State::STATE_B:
//         if (timerChangeState && goCommand)
//         {
//           state = State::STATE_C;
//           Serial.println("\n[+]State change: switching to state C");
//           waitingMessageDisplayed = false;
//           timerChangeState = false;
//         }
//         else if (timerChangeState && !goCommand)
//         {
//           if(!waitingMessageDisplayed)
//           {
//             Serial.println("\nState change paused: waiting to receive go command...");
//             waitingMessageDisplayed = true;
//           }
//         }
//         break;

//       case State::STATE_C:
//         if (timerChangeState && goCommand)
//         {
//           state = State::STATE_A;
//           Serial.println("\n[+]State change: switching to state A");
//           waitingMessageDisplayed = false;
//           timerChangeState = false;
//         }
//         else if (timerChangeState && !goCommand)
//         {
//           if(!waitingMessageDisplayed)
//           {
//             Serial.println("\nState change paused: waiting to receive go command...");
//             waitingMessageDisplayed = true;
//           }
//         }
//         break;
//     }
//   }

//   return 0;
// }

// ISR(TIMER1_COMPA_vect)
// {
//   secondCounter++;
  
//   if(secondCounter >= 5)
//   {
//     timerChangeState = true;
//     secondCounter = 0;
//   }
// }