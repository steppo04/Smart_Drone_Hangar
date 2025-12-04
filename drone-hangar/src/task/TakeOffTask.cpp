#include "task/TakeOffTask.h"
#include "kernel/Logger.h"

#define T1_EXIT_TIME 15000  // max time allowed for drone to exit hangar in ms
#define DOOR_TIME  5000     // time to open/close door in ms
#define COMPLETED_WAIT 2000 // tempo di attesa nello stato completed prima di reset

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
    // Gestione lampeggio LED
    if (state == PREPARING || state == TAKING_OFF || state == COMPLETED) {
        hangar->blinkLed();
    }
    
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
            }
            
            // Sicurezza: ribadiamo la posizione ad ogni ciclo (o almeno la prima volta)
            // Farlo continuamente assicura che il servo raggiunga la posizione     
            
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
            
            // NOTA: Qui il motore è OFF, quindi sensorsCanBeUsed() è TRUE e sync() aggiorna le distanze.
            bool distanceOk = hangar->isDroneOutside();
            bool timeOk = elapsedTimeInState() > T1_EXIT_TIME;
            
            if (distanceOk) {
                Logger.log(F("[TO]: Drone detected OUTSIDE. Closing door."));
                setState(CLOSING_DOOR);
            } else if (timeOk) {
                Logger.log(F("[TO]: Timeout reached (15s). Force closing door."));
                setState(CLOSING_DOOR);
            }
            break;
        }

        case CLOSING_DOOR: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: CLOSING_DOOR state entered."));
                hangar->activateDoor(); // Motore ON
            }
            // FIX CRUCIALE: Chiamiamo closeDoor() continuamente, non solo all'ingresso.
            // Se il servo ha perso il primo comando all'accensione, questo lo corregge.
            hangar->closeDoor(); 
            
            if (elapsedTimeInState() > DOOR_TIME) {
                Logger.log(F("[TO]: Door closed. Operation completed."));
                hangar->deactivateDoor(); // Motore OFF
                setState(COMPLETED);
            }
            break;
        }
        
        case COMPLETED: {
            if(checkAndSetJustEntered()) {
                Logger.log(F("[TO]: COMPLETED state entered."));
                panel->displayDroneOut();
                hangar->stopBlinkLed();
            }
            // FIX CRUCIALE: Tornare a IDLE dopo un breve lasso di tempo
            // Altrimenti il sistema è morto fino al riavvio manuale
            if (elapsedTimeInState() > COMPLETED_WAIT) {
                 Logger.log(F("[TO]: Resetting system to IDLE."));
                 setState(IDLE);
            }
            break;
        }
    }
}