#include "ServoMotorImpl.h"
#include "Arduino.h"

// Definiamo dei limiti "sicuri" per la maggior parte dei servo
// Se il servo non fa tutti i 180 gradi, puoi provare ad abbassare MIN a 600 e alzare MAX a 2400
#define MIN_PULSE 750   // Era 544 (troppo basso per molti servo)
#define MAX_PULSE 2250  // Era 2400

ServoMotorImpl::ServoMotorImpl(int pin){
  this->pin = pin;  
  _on = false;
  _angle = 0; // Default a chiuso
} 

void ServoMotorImpl::on(){
  if (!_on) {
    // 1. Attacca il motore
    motor.attach(pin);
    _on = true;
    
    // 2. Forza immediatamente la scrittura dell'ultimo angolo salvato.
    // Questo evita che il servo salti in posizioni strane o di default.
    setPosition(_angle);
  }
}

bool ServoMotorImpl::isOn(){
  return _on;
}

void ServoMotorImpl::setPosition(int angle){
	if (angle > 180){
		angle = 180;
	} else if (angle < 0){
		angle = 0;
	}
  // 750 -> 0, 2250 -> 180 
  // 750 + angle*(2250-750)/180
  // updated values: min is 544, max 2400 (see ServoTimer2 doc)
  float coeff = (2400.0-544.0)/180;
  motor.write(544 + angle*coeff); 
}

void ServoMotorImpl::off(){
  if (_on) {
    _on = false;
    motor.detach();    
  }
}