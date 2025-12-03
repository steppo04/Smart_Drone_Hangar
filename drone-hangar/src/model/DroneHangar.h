#ifndef DRONEHANGAR_H
#define DRONEHANGAR_H
#include "Dashboard.h"
#include <Arduino.h>
#include "config.h"
#include "HWPlatform.h"

class DroneHangar {
  public:
    DroneHangar(HWPlatform* hw);
    void init();

    //sensors
    float getDroneDistance();
    float getTemperature();
    bool isTempHigh();
    bool isTempVeryHigh();
    bool isTempOk();

    //drone logic state
    bool isDroneInside();
    bool isDroneOutside();
    bool isInPreAlarm();
    bool isInAlarm();

    //actuators
    void openDoor();
    void closeDoor();
    void activateDoor();
    void deactivateDoor();

    //sensors synchronization
    void sync();

    //internal data accessors
    float getTemperature();
    float getDroneDistance();
    bool isDroneDetected();
    bool isHangarPreAlarm();
    bool isHangarAlarmed();

    //other
    void blinkLed();
    void reset();
    void setPreAlarm();
    void setAlarm();

    void setAllowNewOperations(bool allowed);
    bool isNewOperationAllowed();

  private:
    bool sensorsCanBeUsed();
    String stateToString();

    Dashboard* pDashboard;
    HWPlatform* pHW;
    bool dronePIRDetected;
    float droneDistance;
    float temperature;
    bool hangarAlarmed;
    bool hangarPreAlarm;
    bool isHangarOk;
    bool allowNewOperations;

    unsigned long lastToggleTimeL2;
    unsigned long currentTimeL2;
    bool led2State;
};
#endif