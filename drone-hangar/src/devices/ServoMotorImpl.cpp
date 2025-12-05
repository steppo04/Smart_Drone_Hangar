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
  // 1. Limita l'input tra 0 e 180 per sicurezza
  if (angle > 180) angle = 180;
  if (angle < 0) angle = 0;
  
  // 2. Memorizza l'angolo (fondamentale per quando fai on/off)
  this->_angle = angle;

  // 3. CALCOLO SICURO PER MS18
  // I microservi come MS18 lavorano bene tra 750us e 2250us.
  // 544us è troppo basso e lo manda in blocco.
  // map(valore, min_in, max_in, min_out, max_out)
  
  int pulseWidth = map(angle, 0, 180, 750, 2250); 
  
  // 4. Invia comando solo se attivo
  if (_on) {
     motor.write(pulseWidth);
  }
}

void ServoMotorImpl::off(){
  if (_on) {
    _on = false;
    motor.detach();    
  }
}