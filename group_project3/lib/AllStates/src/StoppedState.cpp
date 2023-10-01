#include "StoppedState.h"

void StoppedState::on_entry() {
  Serial.println("Stopped state: Device is stopped.");
  //  LED a 2 Hz
}

void StoppedState::handle_commands() {
  clear fault
  Serial.println("Stopped state: Handling commands...");
}

State* StoppedState::process_commands() {
  
  if (/* Condition */) {
    Serial.println("Stopped state: Transitioning to Operational state...");
    return new OperationalState();
  }
  
  return this; // 
}
