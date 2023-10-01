#include <Arduino.h>
#include<StateManger.h>



MyStateMachine::MyStateMachine() {
  currentState = nullptr;
}

MyStateMachine::~MyStateMachine() {
  if (currentState != nullptr) {
    delete currentState;
  }
}

void MyStateMachine::setup() {
  Serial.begin(9600);
  currentState = new InitializationState();
  currentState->on_entry();
}

void MyStateMachine::loop() {

currentState->handle_commands();
  

nextState = currentState->process_commands();
  
if (nextState != currentState) {
transitionToState(nextState);
}
}



void MyStateMachine::transitionToState(State* nextState) {
  currentState->on_exit();
  delete currentState;
  currentState = nextState;
  currentState->on_entry();
}



void MyStateMachine::on_receiving_go() {
  currentState->on_receiving_go();
}

void MyStateMachine::on_receiving_stop() {
  currentState->on_receiving_stop();
}


void MyStateMachine::sendBootUpMessage() {
Serial.println("Boot-up message: Device is ready to receive commands.");
}