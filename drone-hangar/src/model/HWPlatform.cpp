#include "HWPlatform.h"
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "devices/LightSensorImpl.h" 
#include "devices/TempSensorTMP36.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform() {
    pButtonReset = new ButtonImpl(BT_RESET_PIN);
    pLedStart = new Led(LED_START_PIN);
    pLedAction = new Led(LED_ACTION_PIN);
    pLedAlarm = new Led(LED_ALARM_PIN);
    pLcd = new LiquidCrystal_I2C(0x27,20,4); 
    pMotorDoor = new ServoMotorImpl(MOTOR_PIN);
    pPirDetector = new Pir(PIR_PIN);
    pSonarDetector = new Sonar (SONAR_ECHO_PIN,SONAR_TRIG_PIN,10000);
    pTempSensor = new TempSensorTMP36(TEMP_SENSOR_PIN);
}

void HWPlatform::init(){
  pLcd->init();
  pLcd->backlight();
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), wakeUp, RISING); 

  // pPirUserDetector->calibrate();
}

  Button* HWPlatform::getResetButton(){
    return this->pButtonReset;
  }

  Led*  HWPlatform::getStartLed(){
    return this->pLedStart;
  }
  Led*  HWPlatform::getActionLed(){
    return this->pLedAction;
  }

  Led*  HWPlatform::getAlarmLed(){
    return this->pLedAlarm;
  }

  LiquidCrystal_I2C* HWPlatform::getUserPanelLcd(){
    return this->pLcd;
  }

  Pir*  HWPlatform::getDetectorPir(){
    return this->pPirDetector;
  }

  Sonar* HWPlatform::getDetectorSonar(){
    return this->pSonarDetector;
  }

  TempSensor* HWPlatform::getTempSensor(){
    return this->pTempSensor;
  }

  ServoMotor* HWPlatform::getDoorMotor(){
    return this->pMotorDoor;
  }

  /*void HWPlatform::test(){
  pPirDetector->sync();
  pButtonReset->sync();
  bool detected = pPirDetector->isDetected();
  float dist = pSonarDetector->getDistance();
  bool pressed = pButtonReset->isPressed();
  float temp = pTempSensor->getTemperature();  
  pLedAction->switchOn();
  pLedAlarm->switchOn();
  pLedStart->switchOn();
  pLcd->setCursor(0,0);
  pLcd->print("testHW Drone Hangar");
  pMotorDoor->on();
  pMotorDoor->setPosition(90);
  Logger.log("Pir: " + String((detected ? "detected" : "not detected")));
  Logger.log("Sonar: " + String(dist) + "m");
  Logger.log("Button Reset: " + String(pressed ? "pressed" : " not pressed"));
  Logger.log("Temperature: " + String(temp) + "°C");
  delay(1000);
  pMotorDoor->setPosition(0);
  delay(1000);
  pMotorDoor->off();
  pLcd->clear();
  pLedAction->switchOff();
  pLedAlarm->switchOff();
  pLedStart->switchOff();
}*/

void HWPlatform::test() {
    pMotorDoor->on(); 

    Logger.log("Test: Vado a 150 (Aperto soft)...");
    pMotorDoor->setPosition(150); // Non usare il for, vai diretto per testare
    delay(2000); // Aspetta bene

    Logger.log("Test: Torno a 30 (Chiuso soft)...");
    pMotorDoor->setPosition(30);
    delay(2000);

    Logger.log("Test: Ciclo finito.");
}