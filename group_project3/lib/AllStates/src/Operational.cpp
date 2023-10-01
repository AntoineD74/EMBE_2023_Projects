#include "OperationalState.h"

void OperationalState::on_entry() {
  Serial.println("Operational state: Device is in operational mode.");
  // Led
}

void OperationalState::handle_commands() {
 
  Serial.println("Operational state: Handling commands...");
}

State* OperationalState::process_commands() {
 
  if (/* Condition to pass PreOperational */) {
    Serial.println("Operational state: Transitioning to Pre-operational state...");
    return new PreOperationalState();
  }
  
  
  if (/* reset */) {
    Serial.println("Operational state: Resetting to Initialization state...");
    return new InitializationState();
  }
  
  return this; 
}
