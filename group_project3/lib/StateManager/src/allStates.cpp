#include "state.h"
#include "Arduino.h"

class Concrete_state_a : public State
{
  public:

    void on_entry() override
    {
      Serial.println("\n[+]Currently in state A");
    }

    void on_exit() override
    {
      Serial.println("Switching to state B");
    }

    void on_receiving_go() override
    {
      Serial.println("Go command received !");
    }

     void on_receiving_stop() override
    {
      Serial.println("\nStop command received ! Waiting for go command to resume...");
    }
};

class Concrete_state_b : public State
{
  public:

    void on_entry() override
    {
      Serial.println("\n[+]Currently in state B");
    }

    void on_exit() override
    {
      Serial.println("Switching to state C");
    }

    void on_receiving_go() override
    {
      Serial.println("Go command received !");
    }

     void on_receiving_stop() override
    {
      Serial.println("\nStop command received ! Waiting for go command to resume...");
    }
};

class Concrete_state_c : public State
{
  public:

    void on_entry() override
    {
      Serial.println("\n[+]Currently in state C");
    }

    void on_exit() override
    {
      Serial.println("Switching to state A");
    }

    void on_receiving_go() override
    {
      Serial.println("Go command received !");
    }

     void on_receiving_stop() override
    {
      Serial.println("\nStop command received ! Waiting for go command to resume...");
    }
};