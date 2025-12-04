/*#include "HangarHealthCheckTask.h"
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
                    Logger.log("[HCH] Normal");
                    pHangar->reset(); 
                    pUserPanel->displayDroneInside(); 
                }
                if (pHangar->isTempHigh()) { // T >= TEMP1
                    setState(PREALARM);
                 }
                break;
            }

            case PREALARM: { 
                if (this->checkAndSetJustEntered()){
                    Logger.log("[HCH] Pre-Alarm: new operations suspended.");
                    pHangar->setPreAlarm()// Deve impostare i LED di PRE-ALARM (L1, L2, L3 OFF
                }
                if (pHangar->isTempOk()) { // T < TEMP1 
                     setState(NORMAL);
                } 
            // Condizione 2: Temperatura molto alta o tempo di PRE-ALARM scaduto (passaggio ad ALARM)
                else if (pHangar->isTempVeryHigh() || // T >= TEMP2 
                    elapsedTimeInState() > T4_MILLIS ) {//??
                    setState(ALARM);
            }
            break;
            }

            case ALARM: {
                if (this->checkAndSetJustEntered()){
                    Logger.log(F("[HCH] CRITICAL ALARM. Reset needed."));
                    // 1. Chiudere la porta HD (se aperta)
                    pHangar->reset();
                    pUserPanel->displayDroneInside();
                    // 2. Accendere L3
                    pHangar->setLedL3(true); 
                    // 3. LCD: ALARM
                    pHangar->setLcdMessage(F("ALARM"));
                    // 4. Sospendere tutte le operazioni
                    pHangar->setAllowNewOperations(false);
                    pHangar->setAlarm(); // Allarme critico globale

                    // 5. Se il drone è fuori, inviare ALARM via DRU (Serial)
                    //if (!pHangar->isDroneDetected()) {
                    //  pHangar->sendAlarmToDRU();
                // }
            

            // L'uscita avviene solo con il pulsante RESET, gestito all'inizio di tick().
            break;
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
}*/