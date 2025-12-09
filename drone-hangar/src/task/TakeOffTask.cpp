#include "task/TakeOffTask.h"
#include "kernel/Logger.h"

#define T1_EXIT_TIME 15000  // max time allowed for drone to exit hangar in ms
#define DOOR_TIME  5000     // time to open/close door in ms
#define COMPLETED_WAIT 10000 // tempo di attesa nello stato completed prima di reset

TakeOffTask::TakeOffTask(DroneHangar* hangar, UserPanel* panel, Dashboard* dashboard) {
    this->hangar = hangar;
    this->panel = panel;
    this->dashboard = dashboard;
    this->state = IDLE;
    this->stateStartTime = 0;
    this->justEntered = true;
}

void TakeOffTask::setState(State newState) {
    this->state = newState;
    this->stateStartTime = millis();
    this->justEntered = true;
}

bool TakeOffTask::checkAndSetJustEntered() {
    if (this->justEntered) {
        this->justEntered = false;
        return true;
    }
    return false;
}

unsigned long TakeOffTask::elapsedTimeInState() {
    return millis() - this->stateStartTime;
}

void TakeOffTask::tick() {
    // Sincronizzazione Hangar e Dashboard
    this->hangar->sync();
    this->dashboard->sync();
    this->dashboard->notifyNewState();

    switch (state) {
        case IDLE: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: IDLE state entered. Waiting for command."));
                panel->displayDroneInside();
            } else if (dashboard->checkAndResetTakeOffRequest()) {
                setState(PREPARING);
            }
            break;
        }

        case PREPARING: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: PREPARING state entered."));
                hangar->setTakeOffInProgress(true);
                hangar->startBlinkLed();
                panel->displayTakeOff();
            }
            // Transizione immediata all'apertura
            setState(OPENING_DOOR);
            break;
        }

        case OPENING_DOOR: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: OPENING_DOOR state entered."));
                hangar->activateDoor(); // Motore ON
                hangar->openDoor();
            }  
            
            if (elapsedTimeInState() > DOOR_TIME) {
                hangar->deactivateDoor(); // Motore OFF per risparmiare energia e attivare sensori
                Logger.log(F("[TO]: Door opened. Waiting for drone exit."));
                setState(TAKING_OFF);
            }
            break;
        }

        case TAKING_OFF: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: TAKING_OFF state entered. Sensors Active."));
            }
            bool distanceOk = hangar->isDroneOutside();
            
            if (distanceOk) {
                setState(TIMING);
            }
            break;
        }

        case TIMING: {
            if (elapsedTimeInState() >= T1_EXIT_TIME) {
                Logger.log(F("[TO]: Drone exited successfully."));
                setState(CLOSING_DOOR);
            }
            break;
        }

        case CLOSING_DOOR: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: CLOSING_DOOR state entered."));
                hangar->activateDoor(); // Motore ON
                hangar->closeDoor(); 
            }
            
            if (elapsedTimeInState() > DOOR_TIME) {
                hangar->deactivateDoor(); // Motore OFF
                Logger.log(F("[TO]: Door closed. Operation completed."));
                setState(DRONE_OUT);
            }
            break;
        }
        
        case DRONE_OUT: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: DRONE_OUT state entered."));
                hangar->setTakeOffInProgress(false);
                panel->displayDroneOut();
                hangar->stopBlinkLed();
            } else if (dashboard->checkAndResetLandingRequest()) {
                //setState(LANDING);
                //ESEMPIO:
                hangar->setLandingInProgress(true);
                if(hangar->isDroneInside()) {
                    Logger.log(F("[TO]: Drone is back inside, transitioning to IDLE."));
                    setState(IDLE);
                }
            }
            break;
        }

        /*case LANDING: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: LANDING state entered."));
            }
            if(hangar->isDroneDetected()) {
                Logger.log(F("[TO]: Drone landing detected, transitioning to IDLE."));
                setState(IDLE);
            }
            break;
        }*/
    }
}