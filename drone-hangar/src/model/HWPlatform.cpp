#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/Led.h"
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
    pSonarDetector = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, 10000);
    pTempSensor = new TempSensorTMP36(TEMP_SENSOR_PIN);
}
