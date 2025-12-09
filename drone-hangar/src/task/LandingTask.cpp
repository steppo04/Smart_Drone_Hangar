#include "LandingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/UserPanel.h"


#define DOOR_TIME  5000      
#define T2_ENTER_TIME 10000

LandingTask::LandingTask(DroneHangar* hangar, UserPanel* pPanel, Dashboard* dashboard) {
    this->hangar = hangar;
    this->dashboard = dashboard;
    this->pPanel = pPanel;
    this->state = WAITING_FOR_LANDING;
    this->stateStartTime = 0;
    this->justEntered = true;

}

void LandingTask::setState(State newState) {
    this->state = newState;
    this->stateStartTime = millis();
    this->justEntered = true;
}

bool LandingTask::checkAndSetJustEntered() {
    if (this->justEntered) {
        this->justEntered = false;
        return true;
    }
    return false;
}

unsigned long LandingTask::elapsedTimeInState() {
    return millis() - this->stateStartTime;
}

void LandingTask::tick() {
    this->hangar->sync();
    this->dashboard->sync();
    this->dashboard->notifyNewState();

    switch (state) {
        case WAITING_FOR_LANDING:
            if (hangar->isDroneDetected() && !hangar->isHangarPreAlarm() && !hangar->isHangarAlarmed() && hangar->isLandingInProgress()) {
                Logger.log(F("[LN]: Drone detected inside hangar, opening door"));
                pPanel->displayLanding();
                hangar->openDoor();
                hangar->startBlinkLed();
                
                setState(DOOR_OPENING);
            }
            break;

        case DOOR_OPENING:
            if (elapsedTimeInState() >= DOOR_TIME) {
                Logger.log(F("[LN]: Door opened, landing drone"));
                //pDroneHangar->stopOpeningDoor();
                
                setState(LANDING);
            }
            break;

        case LANDING: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[LN]: LANDING state entered."));
            }
            bool distanceOk = hangar->isDroneInside();

            if(distanceOk) {
                Logger.log(F("[LN]: Start timing for closing door"));
                setState(TIMING);
            }
            break;
        }
        
        case TIMING: {
            if(elapsedTimeInState() >= T2_ENTER_TIME) {
                Logger.log(F("[LN]: Drone has landed inside hangar"));
                
                setState(DOOR_CLOSING);
            }
            break;
        }

        case DOOR_CLOSING:
            if (elapsedTimeInState() >= DOOR_TIME) {
                Logger.log(F("[LN]: Door closed, drone inside hangar."));
                
                //hangar->stopClosingDoor();
                
                setState(DRONE_INSIDE);
            }
            break;

        case DRONE_INSIDE:
            if(checkAndSetJustEntered()) {
                Logger.log(F("[LN]: DRONE_INSIDE state entered."));
                hangar->setLandingInProgress(false);
                pPanel->displayDroneInside();
                hangar->stopBlinkLed();
            } else if (dashboard->checkAndResetTakeOffRequest()) {
                hangar->setTakeOffInProgress(true);
            } else if(hangar->isLandingInProgress()) {
                setState(WAITING_FOR_LANDING);
            }
            break;
    }
}