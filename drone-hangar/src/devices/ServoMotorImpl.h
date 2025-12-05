#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include "ServoMotor.h"
#include <Arduino.h>
#include "ServoTimer2.h"

class ServoMotorImpl: public ServoMotor {

public:
  ServoMotorImpl(int pin);

  void on();
  bool isOn();
  void setPosition(int angle);
  void off();
    
private:
  int pin; 
  bool _on;
  int _angle; // <--- NUOVA VARIABILE PER RICORDARE LA POSIZIONE
  ServoTimer2 motor; 
};

#endif