#include <Arduino.h>
#include <StateManager.h>


StateManager::StateManager() : state_(nullptr), currentStateIndex(0), blinking_period(0), motor(2, 1000), led(5)
{
  // MotorController ;
  
  led.init();
  motor.init(21);
  // motor.set(0.0);

  this->transitionToState(new InitializationState);
  this->loopAction();

  this->transitionToState(new PreOperationalState);
  currentStateIndex = 1;

  Serial.print("\nEnter a value for Kp: ");
  char chars[20];
  char character;
  int i = 0;
  while(1){
    if(Serial.available())
    {
      character = Serial.read();
      if(character == 13){ break; }
      chars[i] = character;
      i++;
    }
    if(i==20){ break; }
  }
  float kp = strtod(chars, nullptr);
  Serial.println(kp);

  Serial.print("Enter a value for Ti: ");
  char chars2[20];
  char character2;
  int j = 0;
  while(1){
    if(Serial.available())
    {
      character2 = Serial.read();
      if(character2 == 13){ break; }
      chars2[j] = character2;
      j++;
    }
    if(j==20){ break; }
  }
  float ti = strtod(chars2, nullptr);
  Serial.println(ti);

  motor.speedController.changeParameters(kp, ti);

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
      blinking_period = 0;
      led.set_hi();
    }

    else if (cmd == 'r'){
      motor.set(0.0);
      motor.brake();
      this->transitionToState(new InitializationState);
      this->loopAction();
      this->transitionToState(new PreOperationalState);
      this->loopAction();
      currentStateIndex = 1;
      blinking_period = 48; //1Hz

      Serial.print("\nEnter a value for Kp: ");
      char chars[20];
      char character;
      int i = 0;
      while(1){
        if(Serial.available())
        {
          character = Serial.read();
          if(character == 13){ break; }
          chars[i] = character;
          i++;
        }
        if(i==20){ break; }
      }
      double kp = strtod(chars, nullptr);
      Serial.println(kp);

      Serial.print("Enter a value for Ti: ");
      char chars2[20];
      char character2;
      int j = 0;
      while(1){
        if(Serial.available())
        {
          character2 = Serial.read();
          if(character2 == 13){ break; }
          chars2[j] = character2;
          j++;
        }
        if(j==20){ break; }
      }
      double ti = strtod(chars2, nullptr);
      Serial.println(ti);

      motor.speedController.changeParameters(kp, ti);
    }
    
    else if (cmd == 'S'){
      this->transitionToState(new StoppedState);
      currentStateIndex = 3;
      blinking_period = 24; //2Hz
    }
    
    else if (cmd == 'p'){
      this->transitionToState(new PreOperationalState);
      currentStateIndex = 1;
      blinking_period = 48; //1Hz
    }

    else{
      Serial.println("Command unknown ...");
    }
}

int StateManager::get_blinking_period(){
  return blinking_period;
}