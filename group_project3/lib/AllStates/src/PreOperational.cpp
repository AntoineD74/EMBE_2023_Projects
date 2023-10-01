#include <Arduino.h>
#include "PreOperational.h"

void PreOperationalState::on_entry() {
  Serial.println("\n[+]Pre-operational state: Device is in pre-operational configuration.");
  // Led 1Hz
}

void PreOperationalState::action_looped(MotorController *motor){

}