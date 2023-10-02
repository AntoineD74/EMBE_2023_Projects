#include "StoppedState.h"
#include "Arduino.h"

void StoppedState::on_entry() {
  Serial.println("\n[+]Stopped state: Device is stopped.");
  //  LED a 2 Hz
}

void StoppedState::action_looped(MotorController *motor){
  motor->set(0.0);
  motor->brake();
}