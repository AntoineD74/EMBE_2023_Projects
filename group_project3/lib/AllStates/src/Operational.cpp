#include <Arduino.h>
#include "Operational.h"

void OperationalState::on_entry() {
  Serial.println("\n[+]Operational state: Device is in operational mode.");
}

void OperationalState::action_looped(MotorController *motor){
  motor->updatePwm();
  Serial.println(motor->currentSpeed);
  // Serial.println(motor->speedController.updatePwm( motor->currentSpeed));
}