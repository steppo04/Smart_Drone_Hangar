#ifndef HANGARHEALTHCHECKTASK_H
#define HANGARHEALTHCHECKTASK_H

#include "kernel/Task.h"
#include "model/DroneHangar.h"
#include "model/UserPanel.h"

class HangarHealthCheckTask : public Task {
    public:
        /**
         * @brief Stati della FSM per il controllo della salute dell'Hangar (temperatura).
         */
        enum {
            NORMAL,       // T < Temp1
            PREALARM,     // Temp1 <= T < Temp2, o T >= Temp1 ma T4 non è scaduto
            ALARM         // T >= Temp2 o T >= Temp1 per un tempo > T4
        };
        
        /**
         * @brief Costruttore della Task.
         * * @param pHangar Puntatore al sistema Hangar per sensori e attuatori.
         * @param pUserPanel Puntatore al pannello utente per il controllo del pulsante RESET.
         */
        HangarHealthCheckTask(DroneHangar* pHangar, UserPanel* pUserPanel);
        
        /**
         * @brief Metodo principale eseguito periodicamente dallo Scheduler.
         * Contiene la logica della FSM.
         */
        void tick();

    private:
        DroneHangar* pHangar;
        UserPanel* pUserPanel; 
        
        int state;
        long stateTimestamp;
        bool justEntered;

        // Metodi helper per la gestione della FSM
        
        /**
         * @brief Imposta il nuovo stato della FSM e resetta il timestamp.
         * * @param s Il nuovo stato.
         */
        void setState(int s);
        
        /**
         * @brief Calcola il tempo trascorso nello stato corrente.
         * * @return long Tempo trascorso in millisecondi.
         */
        long elapsedTimeInState();
        
        /**
         * @brief Verifica se la FSM è appena entrata nello stato e resetta il flag.
         * * @return true Se è appena entrata nello stato.
         * @return false Altrimenti.
         */
        bool checkAndSetJustEntered();
};

#endif // HANGARHEALTHCHECKTASK_H