#include "DroneHangar.h"
#include "kernel/Logger.h"

#define D1 100.0  // distance threshold for drone detection in cm
#define D2 30.0   // distance threshold for drone inside hangar in cm


DroneHangar::DroneHangar(HWPlatform* hw) {
    this->pHW = hw;
}

void DroneHangar::init() {
    filteredDistance = 0.0;
    temperature = 20.0;
    droneDistance = 10.0;
    currentTimeL2 = millis();
    this->reset();
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
    pHW->getDoorMotor()->setPosition(180);
}

void DroneHangar::closeDoor() {
    pHW->getDoorMotor()->setPosition(0);
}

void DroneHangar::activateDoor() {
    pHW->getDoorMotor()->on();
}

void DroneHangar::deactivateDoor() {
    pHW->getDoorMotor()->off();
}

void DroneHangar::sync() {
    float temp = temperature;
    bool droneDetected = dronePIRDetected;

    if (sensorsCanBeUsed()) {

        float NewDist = pHW->getDetectorSonar()->getDistance();
        float currentTemp = pHW->getTempSensor()->getTemperature();
        bool currentPir = pHW->getDetectorPir()->isDetected();
        float inputDist = 0;
        
        if (NewDist == NO_OBJ_DETECTED || NewDist <= 0 ) { // Se non rileva nulla, fingiamo sia a 200cm
            inputDist = 200.0;
        } else {// Se vede qualcosa, usiamo il dato reale  
            inputDist = NewDist;
        }

        //applichiamo il filtro in modo di attenuare il jittering causato dal continuo aggiornamento 
        float alpha = 0.1;
        filteredDistance = (alpha * inputDist) + ((1.0 - alpha) * filteredDistance);

        droneDistance = filteredDistance;
        temperature = currentTemp;
        dronePIRDetected = currentPir;
    }

    static unsigned long counter = 0;
    counter++;
    if (counter % 30 == 0) {
        Logger.log("[DH] dist=" + String(droneDistance) +
                   " temp=" + String(temp) +
                   " pir=" + String(droneDetected) +
                   " state=" + stateToString());
    }

    if(isHangarOk && (takeOffInProgress || landingInProgress)) {
        blinkLed();
    } else {
        stopBlinkLed();
    }
}

//internal data accessors
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
    hangarPreAlarm = false;
    hangarAlarmed = false;
    dronePIRDetected = false;
    allowNewOperations = true;
    takeOffInProgress = false;
    landingInProgress = false;
    lastToggleTimeL2 = 0;
    led2State = false;
    currentTimeL2 = millis();
    
    pHW->getDoorMotor()->on();
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

    activateDoor();
    deactivateDoor();

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
    if (currentTimeL2 - lastToggleTimeL2 >= BLINK_PERIOD_MS) {
        led2State = !led2State;
        if (led2State) {
            pHW->getActionLed()->switchOn();
        } else {
            pHW->getActionLed()->switchOff();
        }
        lastToggleTimeL2 = currentTimeL2;
    }
}
void DroneHangar::startBlinkLed() {
    lastToggleTimeL2 = millis();
    led2State = true;
}

void DroneHangar::stopBlinkLed() {
    led2State = false;
    pHW->getActionLed()->switchOff();
}

void DroneHangar::setTakeOffInProgress(bool inProgress) {
    this->takeOffInProgress = inProgress;
}

bool DroneHangar::isTakeOffInProgress() {
    return this->takeOffInProgress;
}

void DroneHangar::setLandingInProgress(bool inProgress) {
    this->landingInProgress = inProgress;
}

bool DroneHangar::isLandingInProgress() {
    return this->landingInProgress;
}

int DroneHangar::getDroneStateCode() {
    // Mappatura Stati: 0=REST, 1=TAKEOFF, 2=OUT, 3=LANDING
    if (takeOffInProgress) {
        return 1; 
    } else if (landingInProgress) {
        return 3;
    } else if (isDroneInside()) {
        return 0;
    } else {
        return 2; 
    }
}

int DroneHangar::getHangarStateCode() {
    if (isHangarAlarmed()) {
        return 2; // ALARM
    } else if (isHangarPreAlarm()) {
        return 1; // PRE-ALARM
    } else {
        return 0; // NORMAL
    }
}

//private methods
bool DroneHangar::sensorsCanBeUsed(){
  return !pHW->getDoorMotor()->isOn();
}

String DroneHangar::stateToString() {
    String state = "";
    if (isHangarOk) {
        state = "OK";
    } else if (hangarPreAlarm) {
        state = "PRE-ALARM";
    } else if (hangarAlarmed) {
        state = "ALARM";
    } else {
        state = "UNKNOWN";
    }
    return state;
}
