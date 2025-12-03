#include "task/TakeOffTask.h"

#define T1_EXIT_TIME 15000  // max time allowed for drone to exit hangar in ms

TakeOffTask::TakeOffTask(DroneHangar* hangar, UserPanel* panel, Dashboard* dashboard) {
    this->hangar = hangar;
    this->panel = panel;
    this->dashboard = dashboard;
    this->state = PREPARING;
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
    this->hangar->blinkLed();

    switch (state) {
        this->hangar->sync();
        this->dashboard->sync();
        this->dashboard->notifyNewState();

        case IDLE: {
            if(checkAndSetJustEntered()) {
                Logger.log("[TO]: IDLE state entered.");
            } else if (dashboard->checkAndResetTakeOffRequest()) {
                setState(PREPARING);
            }
            break;
        }

        case PREPARING: {
            if(checkAndSetJustEntered()) {
                Logger.log("[TO]: PREPARING state entered.");
                hangar->openDoor();
                panel->displayTakeOff();
            }
            break;
        }

        case TAKING_OFF: {
            if(checkAndSetJustEntered()) {
                Logger.log("[TO]: TAKING_OFF state entered.");
            } else {
                bool distanceOk = hangar-> isDroneOutside();
                bool timeOk = elapsedTimeInState() > T1_EXIT_TIME;
                if (distanceOk || timeOk) {
                    setState(COMPLETED);
                }
            }
            break;
        }

        case COMPLETED: {
            if(checkAndSetJustEntered()) {
                Logger.log("[TO]: COMPLETED state entered.");
                hangar->closeDoor();
                hangar->deactivateDoor();
                panel->displayDroneOut();
            }
            setState(IDLE);
            break;
        }
    }
}
