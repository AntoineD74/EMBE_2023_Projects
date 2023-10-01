#include <Arduino.h>
#include "Operational.h"

void OperationalState::on_entry() {
  Serial.println("\n[+]Operational state: Device is in operational mode.");
  // Led
}

void OperationalState::action_looped(MotorController *motor){
  motor->updatePwm();
}