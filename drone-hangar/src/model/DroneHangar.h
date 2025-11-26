#ifndef DRONEHANGAR_H
#define DRONEHANGAR_H
#include <Arduino.h>
#include "config.h"

class DroneHangar {
  public:
    DroneHangar(/*HWplatform* hw*/);
    void init();

    //sensors
    float getDroneDistance();
    bool isDroneDetected();
    float getTemperature();
    bool isTempHigh();
    bool isTempVeryHigh();
    bool isTempOk();

    //drone logic state
    bool isDroneInside();
    bool isDroneOutside();
    bool isDoorOpen();
    bool isDoorClosed();
    bool isInPreAlarm();
    bool isInAlarm();

    //actuators
    void openDoor();
    void closeDoor();
    void stopDoor();

    //sensors synchronization
    void syncSensors();

    //internal data accessors
    float getTemperature();
    float getDroneDistance();
    bool isDroneDetectedByPIR();
    bool isHangarAlarmed();

  private:
    void setLed1On();
    void setLed1Off();
    void setLed2TakeoffBlinking();
    void setLed2TakeoffOff();
    void setLed3AlarmOn();
    void setLed3AlarmOff();

    HWPlatform* pHW;
    bool droneDetected;
    float droneDistance;
    float temperature;
    bool hangarAlarmed;

};
#endif