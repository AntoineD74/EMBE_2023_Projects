#include "motor_controller.h"
#include <avr/io.h>
#include <avr/interrupt.h>

MotorController::MotorController(int pinNumber, int target): pin(pinNumber), timer(), speedTarget(target), kp(0.001)
{
}

void MotorController::init(int period_ms = 1000)
{
    timer.init(period_ms, 0.5);
    pin.init();

    DDRD &= ~(1 << DDD2); 
    PORTD |= (1 << PORTD2);
    EICRA |= (1 << ISC01) | (1 << ISC00); // set INT0 on rising edge
    EIMSK |= (1 << INT0);
    sei();
}

void MotorController::set(float duty_cycle)
{
    timer.set_duty_cycle(duty_cycle);
}


void MotorController::updatePwm()
{
    double error;
    if(this->speedTarget > currentSpeed){ 
        error = speedTarget - currentSpeed;
    } else{ 
        error = currentSpeed - speedTarget;
    }
    this->set(kp * error);
}


void MotorController::brake(){
    this->pin.set_lo();
}