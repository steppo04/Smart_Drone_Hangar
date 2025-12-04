/*#include "LandingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/UserPanel.h"


#define DOOR_TIME  1000      
#define TIME_T2 5000 
#define SLEEP_TIMEOUT 120000

LandingTask::LandingTask(DroneHangar* pDroneHangar, UserPanel* pPanel): 
    pDroneHangar(pDroneHangar), pPanel(pPanel) {
    setState(WAITING_FOR_LANDING);
    t2TimerActive = false;
}

void LandingTask::tick() {

    if (state == DOOR_OPENING || state == LANDING || state == DOOR_CLOSING) {
        pDroneHangar->blinkLed();
    }

    pPanel->sync();
    checkAndSetJustEntered();

    switch (state) {
        case WAITING_FOR_LANDING:
            if (pDroneHangar->isDroneDetected() && 
                !pDroneHangar->isHangarPreAlarm() && 
                !pDroneHangar->isHangarAlarmed()) {
                
                Logger.log("Drone detected inside hangar, opening door");
                pPanel->displayLanding();
                
                pDroneHangar->openDoor();
                pDroneHangar->startBlinkLed();
                
                setState(DOOR_OPENING);
            }
            break;

        case DOOR_OPENING:
            if (elapsedTimeInState() >= DOOR_TIME) {
                Logger.log("Door opened, landing drone");
                //pDroneHangar->stopOpeningDoor();
                t2TimerActive = false;
                
                setState(LANDING);
            }
            break;

        case LANDING:
            if (pDroneHangar->isDroneInside()) {
                if (!t2TimerActive) {
                    t2StartTime = millis();
                    t2TimerActive = true;
                    Logger.log("Drone entered threshold D2. Verifying T2...");
                } else {
                    unsigned long elapsed = millis() - t2StartTime;
                    if (elapsed >= TIME_T2) {
                        Logger.log("T2 Confirmed. Drone Landed.");
                        
                        pDroneHangar->closeDoor();
                        setState(DOOR_CLOSING);
                    }
                }
            }
            break;

        case DOOR_CLOSING:
            if (elapsedTimeInState() >= DOOR_TIME) {
                Logger.log("Door closed, drone inside hangar.");
                
                //pDroneHangar->stopClosingDoor(); 
                pDroneHangar->stopBlinkLed();
                
                setState(DRONE_INSIDE);
            }
            break;

        case DRONE_INSIDE:
             pPanel->displayDroneInside();
            break;
    }
}*/