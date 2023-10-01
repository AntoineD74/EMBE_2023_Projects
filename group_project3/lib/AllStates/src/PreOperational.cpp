#include "PreOperationalState.h"

void PreOperationalState::on_entry() {
  Serial.println("Pre-operational state: Device is in pre-operational configuration.");
  // Led 1Hz
}

void PreOperationalState::handle_commands() {
  
  Serial.println("Pre-operational state: Handling commands...");
}

State* PreOperationalState::process_commands() {
  
  if (/* Condition to pass to Operational */) {
    Serial.println("Pre-operational state: Transitioning to Operational state...");
    return new OperationalState();
  }
  
  
  if (/* reset */) {
    Serial.println("Pre-operational state: Resetting to Initialization state...");
    return new InitializationState();
  }
  
  return this; // same state
}