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