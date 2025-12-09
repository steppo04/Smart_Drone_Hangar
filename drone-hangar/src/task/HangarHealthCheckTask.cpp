#include "HangarHealthCheckTask.h"
#include "model/DroneHangar.h" 
#include "model/UserPanel.h"
#include <Arduino.h>
#include "kernel/Logger.h"

#define T4_MILLIS 10000
HangarHealthCheckTask::HangarHealthCheckTask(DroneHangar* pHangar, UserPanel* pUserPanel) {
    this->pHangar = pHangar;
    this->pUserPanel = pUserPanel;
    this->state = NORMAL;
    
}

void HangarHealthCheckTask::tick() {
    pUserPanel->sync();
    
    switch (state){ 
            case NORMAL: {
                if (this->checkAndSetJustEntered()){
                    Logger.log("[HCH] Normal state: monitoring temperature.");
                    pHangar->reset(); 
                    if(pHangar -> isDroneInside()) {
                        pUserPanel->displayDroneInside();
                    } else if (pHangar -> isDroneOutside()) {
                        pUserPanel->displayDroneOut();
                    }
                }
                if (pHangar->isTempHigh()) { // T >= TEMP1
                    setState(PREALARM);
                 }
                break;
            }

            case PREALARM: { 
                if (this->checkAndSetJustEntered()){
                    Logger.log("[HCH] Pre-Alarm: new operations suspended.");
                    pHangar->setPreAlarm();// Deve impostare i LED di PRE-ALARM (L1, L2, L3 OFF)
                }
                if (pHangar->isTempOk()) { // T < TEMP1 
                     setState(NORMAL);
                }
                else if (pHangar->isTempVeryHigh() || elapsedTimeInState() > T4_MILLIS ) {
                    setState(ALARM);
            }
            break;
            }

            case ALARM: {
                if (this->checkAndSetJustEntered()){
                    Logger.log(F("[HCH] CRITICAL ALARM. Reset needed."));
                    //chiusura porta (se aperta) TO DO
                    pUserPanel->displayDroneInside();
                    pHangar->setAllowNewOperations(false);
                    pHangar->setAlarm();
                    pUserPanel->displayAlarm();

                    break;
                }
                if(pUserPanel->pressedReset()) {
                    Logger.log(F("[HCH] RESET pressed. Returning to NORMAL state."));
                    setState(NORMAL);
            }
        }   
    } 
}


void HangarHealthCheckTask::setState(int s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long HangarHealthCheckTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool HangarHealthCheckTask::checkAndSetJustEntered() {
    bool bak = justEntered;
    if (justEntered) {
        justEntered = false;
    }
    return bak;
}