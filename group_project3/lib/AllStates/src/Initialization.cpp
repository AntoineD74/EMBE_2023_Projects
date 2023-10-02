#include <Arduino.h>
#include "InitializationState.h"

void InitializationState::on_entry() {
  Serial.println("\n[+]Initialization state: Device is initializing");
  Serial.println("Booting ...");

  // OCR2B = OCR2A * 10000000;
}

  
void InitializationState::action_looped(MotorController *motor){
  motor->set(0);
  motor->brake();
}