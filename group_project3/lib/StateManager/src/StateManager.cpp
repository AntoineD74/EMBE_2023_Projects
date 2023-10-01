#include <Arduino.h>
#include<StateManager.h>


StateManager::StateManager() : state_(nullptr), currentStateIndex(0), motor(2, 1000)
{
  // MotorController ;
  motor.init(21);
  motor.set(0.0);

  this->transitionToState(new InitializationState);
  this->transitionToState(new PreOperationalState);
  currentStateIndex = 1;
}

StateManager::~StateManager(){
    delete state_;
}

void StateManager::loopAction() {
    
  state_->action_looped(&motor);
  
}

void StateManager::transitionToState(State* nextState) {
  if (this->state_ != nullptr)
  {
      // this->state_->on_exit();
      delete this->state_;
  }
  state_ = nextState;
  state_->on_entry();
}

void StateManager::receive_command(char cmd)
{
    if(cmd == 's'){
      this->transitionToState(new OperationalState);
      currentStateIndex = 2;
    }

    else if (cmd == 'r'){
      this->transitionToState(new InitializationState);
      this->transitionToState(new PreOperationalState);
      currentStateIndex = 0;
    }
    
    else if (cmd == 'S'){
      this->transitionToState(new StoppedState);
      currentStateIndex = 3;
    }
    
    else if (cmd == 'p'){
      this->transitionToState(new PreOperationalState);
      currentStateIndex = 1;
    }

    else{
      Serial.println("Command unknown ...");
    }
    
}