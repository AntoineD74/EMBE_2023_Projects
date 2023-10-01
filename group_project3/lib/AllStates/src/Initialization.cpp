#include <Arduino.h>
#include "InitializationState.h"

void InitializationState::on_entry() {
  Serial.println("\n[+]Initialization state: Device is initializing");
  Serial.println("Booting ...");
}

  
void InitializationState::action_looped(MotorController *motor){

}