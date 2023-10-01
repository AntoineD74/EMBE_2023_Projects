#include "InitializationState.h"

void InitializationState::on_entry() {
  Serial.println("Initialization state: Device is initializing...");
}

void InitializationState::handle_commands() {
  
  Serial.println("Initialization state: Handling commands...");
}

State* InitializationState::process_commands() {
    Serial.println("Initialization state: Transitioning to Pre-operational state...");
    return new PreOperationalState();
    // Without if condition because the transition is autonomous
}
  
  
  if (/*RESET*/) {
    Serial.println("Initialization state: Resetting to Initialization state...");
    return new InitializationState();
  }
  
  return this; 
}
