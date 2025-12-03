#include "DroneHangar.h"
#include "kernel/Logger.h"

#define D1 100.0  // distance threshold for drone detection in cm
#define D2 30.0   // distance threshold for drone inside hangar in cm
#define TEMP1 30.0   // temperature threshold for high temperature in degree Celsius
#define TEMP2 40.0   // temperature threshold for very high temperature in degree Celsius


DroneHangar::DroneHangar(HWPlatform* hw) {
    this->pHW = hw;
}

void DroneHangar::init() {
    temperature = 20.0;
    droneDistance = 10.0;
    isHangarOk = true;
    hangarPreAlarm = false;
    hangarAlarmed = false;
    dronePIRDetected = false;
    allowNewOperations = true;

    this->reset();
    pHW->getDoorMotor()->on();
    pHW->getStartLed()->switchOn();
    pHW->getActionLed()->switchOff();
    pHW->getAlarmLed()->switchOff();
    pHW->getDoorMotor()->off();
}

//sensors
float DroneHangar::getDroneDistance() {
    return droneDistance;
}

float DroneHangar::getTemperature() {
    return temperature;
}

bool DroneHangar::isTempHigh() {
    return (temperature >= TEMP1);
}

bool DroneHangar::isTempVeryHigh() {
    return (temperature >= TEMP2);
}

bool DroneHangar::isTempOk() {
    return (temperature < TEMP1);
}

//drone logic state
bool DroneHangar::isDroneInside() {
    return (droneDistance <= D2);
}

bool DroneHangar::isDroneOutside() {
    return (droneDistance > D2);
}

bool DroneHangar::isInPreAlarm() {
    return (isTempHigh() && !isTempVeryHigh());
}

bool DroneHangar::isInAlarm() {
    return isTempVeryHigh();
}

//actuators
void DroneHangar::openDoor() {
    if(isHangarOk) {
        pHW->getDoorMotor()->on();
        pHW->getDoorMotor()->setPosition(180);
    }
}

void DroneHangar::closeDoor() {
    pHW->getDoorMotor()->on();
    pHW->getDoorMotor()->setPosition(0);
}

void DroneHangar::activateDoor() {
    pHW->getDoorMotor()->on();
}

void DroneHangar::deactivateDoor() {
    pHW->getDoorMotor()->off();
}

void DroneHangar::sync() {
    float dist = droneDistance;
    float temp = temperature;
    bool droneDetected = dronePIRDetected;

    if (sensorsCanBeUsed()) {

        dist = pHW->getDetectorSonar()->getDistance();
        temp = pHW->getTempSensor()->getTemperature();
        droneDetected = pHW->getDetectorPir()->isDetected();

        if (dist == NO_OBJ_DETECTED) {
            dist = D1;
        }

        droneDistance = dist;
        temperature = temp;
        dronePIRDetected = droneDetected;
    }

    static unsigned long counter = 0;
    counter++;
    if (counter % 30 == 0) {
        Logger.log("[DH] dist=" + String(dist) +
                   " temp=" + String(temp) +
                   " pir=" + String(droneDetected) +
                   " state=" + stateToString());
    }


    // state: OK → PRE-ALARM
    if (isHangarOk && isInPreAlarm()) {

        hangarPreAlarm = true;
        hangarAlarmed = false;
        isHangarOk = false;

        // LED
        pHW->getStartLed()->switchOff();
        pHW->getAlarmLed()->switchOff();
        pHW->getActionLed()->switchOff();
    }

    // state: PRE-ALARM → OK
    if (hangarPreAlarm && isTempOk()) {

        hangarPreAlarm = false;
        hangarAlarmed = false;
        isHangarOk = true;

        // LED
        pHW->getStartLed()->switchOn();
        pHW->getAlarmLed()->switchOff();
    }

    // state: PRE-ALARM → ALARM
    if (hangarPreAlarm && isInAlarm()) {

        hangarAlarmed = true;
        hangarPreAlarm = false;
        isHangarOk = false;

        closeDoor();

        // LED
        pHW->getStartLed()->switchOff();
        pHW->getActionLed()->switchOff();
        pHW->getAlarmLed()->switchOn();
    }

    // state: OK → ALARM 
    if (isHangarOk && isInAlarm()) {

        hangarAlarmed = true;
        hangarPreAlarm = false;
        isHangarOk = false;

        closeDoor();

        pHW->getStartLed()->switchOff();
        pHW->getActionLed()->switchOff();
        pHW->getAlarmLed()->switchOn();
    }

    // state: ALARM → OK
    if (hangarAlarmed && isTempOk()) {

        hangarAlarmed = false;
        isHangarOk = true;

        pHW->getStartLed()->switchOn();
        pHW->getAlarmLed()->switchOff();
    }

    //Motor in action --> LED2 (Action led) ON
    if (pHW->getDoorMotor()->isOn()) {
        pHW->getActionLed()->switchOn();
    } else {
        pHW->getActionLed()->switchOff();
    }
}

//internal data accessors
float DroneHangar::getTemperature() {
    return temperature;
}

float DroneHangar::getDroneDistance() {
    return droneDistance;
}

bool DroneHangar::isDroneDetected() {
    return dronePIRDetected;
}

bool DroneHangar::isHangarPreAlarm() {
    return hangarPreAlarm;
}

bool DroneHangar::isHangarAlarmed() {
    return hangarAlarmed;
}

//other
void DroneHangar::reset() {
    isHangarOk = true;
    pHW->getStartLed()->switchOn();
    pHW->getActionLed()->switchOff();
    pHW->getAlarmLed()->switchOff();
    pHW->getDoorMotor()->off();
}

void DroneHangar::setPreAlarm() {
    hangarPreAlarm = true;
    hangarAlarmed = false;
    isHangarOk = false;

    // LED
    pHW->getStartLed()->switchOff();
    pHW->getAlarmLed()->switchOff();
    pHW->getActionLed()->switchOff();
}

void DroneHangar::setAlarm() {
    hangarAlarmed = true;
    hangarPreAlarm = false;
    isHangarOk = false;

    closeDoor();

    // LED
    pHW->getStartLed()->switchOff();
    pHW->getActionLed()->switchOff();
    pHW->getAlarmLed()->switchOn();
}

void DroneHangar::setAllowNewOperations(bool allowed) {
    this->allowNewOperations = allowed;
}

bool DroneHangar::isNewOperationAllowed() {
    return this->allowNewOperations;
}


void DroneHangar::blinkLed() {
    currentTimeL2 = millis();
    if (currentTimeL2 - lastToggleTimeL2 >= 500) {
        led2State = !led2State;
        if (led2State) {
            pHW->getActionLed()->switchOn();
        } else {
            pHW->getActionLed()->switchOff();
        }
        lastToggleTimeL2 = currentTimeL2;
    }
}


//private methods

/*Durante l’apertura o la chiusura, il servo consuma molta corrente e vibra.
Il sonar (WASTE DETECTOR) e il sensore di temperatura (TEMP) 
sono sensori sensibili ai disturbi elettrici e meccanici 
generati dal movimento del motore.*/
bool DroneHangar::sensorsCanBeUsed(){
  return !pHW->getDoorMotor()->isOn();
}

String DroneHangar::stateToString() {
    if(isHangarOk) {
        return "OK";
    } else if(hangarPreAlarm) {
        return "PRE-ALARM";
    } else if(hangarAlarmed) {
        return "ALARM";
    } else {
        return "UNKNOWN";
    }
}
