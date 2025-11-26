
#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/TempSensor.h"
#include "devices/Led.h"
#include "devices/ServoMotor.h"

class HWPlatform {

public:
    HWPlatform();
    void init();
    void test();

    Button* getResetButton();
    Led* getStartLed();
    Led* getActionLed();
    Led* getAlarmLed();
    LiquidCrystal_I2C* getUserPanelLcd();
    Pir* getDetectorPir();
    Sonar* getDetectorSonar();
    TempSensor* getTempSensor();  
    ServoMotor* getDoorMotor();

private: 
    Button* pButtonReset;
    Led* pLedStart;
    Led* pLedAction;
    Led* pLedAlarm;
    LiquidCrystal_I2C* pLcd; 
    ServoMotor* pMotorDoor;
    Pir* pPirDetector;
    Sonar* pSonarDetector;
    TempSensor* pTempSensor;

};

#endif