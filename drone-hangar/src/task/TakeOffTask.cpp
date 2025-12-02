#include "task/TakeOffTask.h"

#define D1_EXIT_DISTANCE_CM 50 
#define T1_EXIT_TIME_SEC 3

TakeOffTask::TakeOffTask(DroneHangar* hangar, UserPanel* panel, Dashboard* dashboard) {
    this->hangar = hangar;
    this->panel = panel;
    this->dashboard = dashboard;
    this->state = PREPARING;
    this->stateStartTime = 0;
    this->justEntered = true;
    this->lastToggleTimeL2 = 0;
    this->led2State = false;
    this->currentTimeL2 = millis();
}

void TakeOffTask::blinkL2() {
    static unsigned long lastToggleTime = 0;
    static bool ledState = false;
    unsigned long currentTime = millis();
    
    if (currentTime - lastToggleTime >= 500) {
        ledState = !ledState;
        if (ledState) {
            hangar->pHW->getActionLed()->switchOn();
        } else {
            hangar->pHW->getActionLed()->switchOff();
        }
        lastToggleTime = currentTime;
    }
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
    this->blinkL2();

    switch (state) {
        case PREPARING: {
            // Stato iniziale: la task è in attesa di essere attivata esternamente.
            // Quando la richiesta di decollo arriva (es. tramite seriale),
            // il codice che riceve il messaggio DEVE chiamare un metodo (es. startTakeOff())
            // sulla task, che a sua volta chiamerà setState(PREPARING).
            // La Task non fa nulla finché non viene attivata.
            /*if (checkAndSetJustEntered()) {
                // Azioni all'ingresso dello stato PREPARING
            }
            
            // Si passa immediatamente a TAKING_OFF (l'apertura è un'azione istantanea per la FSM)
            setState(TAKING_OFF);*/
            break;
        }

        case TAKING_OFF: {
            /*// Logica di permanenza: Attendere che il drone esca.
            int distance = this->hangar->getDroneDistanceDetector()->getDistance();
            
            if (distance > D1_EXIT_DISTANCE_CM) {
                // Il drone è "lontano", si inizia a contare il tempo T1
                if (checkAndSetJustEntered()) {
                    // Si è appena superata la distanza D1, si riparte con il conteggio
                    this->stateStartTime = millis(); 
                    this->justEntered = false; // Mantenere justEntered a false
                }
                
                if (elapsedTimeInState() >= T1_EXIT_TIME_SEC * 1000) {
                    // Tempo T1 trascorso: il drone è uscito.
                    setState(COMPLETED);
                }
            } else {
                // Il drone è rientrato (o non è ancora uscito stabilmente).
                // Si resetta il contatore T1, si torna alla condizione di attesa.
                this->stateStartTime = millis();
                this->justEntered = true;
            }
            
            // Controlli di allarme: se è attivo l'allarme Temp2, si forzano le chiusure.
            if (this->hangar->isAlarm()) {
                // La specifica dice: se un take-off è già in corso, è consentito completare,
                // ma in caso di ALLARME (Temp2 > T4) la porta DEVE essere chiusa.
                this->hangar->getHangarDoor()->close();
                // NON cambiamo stato in ALARM, ma lo completiamo, passando il controllo.
            }*/
            break;
        }

        case COMPLETED: {
            /*if (checkAndSetJustEntered()) {
                // Azioni all'ingresso dello stato COMPLETED
                this->hangar->getHangarDoor()->close();
                this->panel->displayMessage("DRONE OUT");
                this->hangar->setTakeOffInProgress(false);
                this->hangar->setDroneInside(false); // Stato globale: Drone OUT
                this->panel->getL1()->switchOff(); // L1 deve essere spento quando il drone è fuori
                
                // La Task deve autosospendersi (tornare a INIT)
                setState(INIT);
            }*/
            break;
        }
    }
}
