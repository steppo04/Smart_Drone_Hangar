#include "Dashboard.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "model/DroneHangar.h"

Dashboard::Dashboard(DroneHangar* pHangar) {
    this->pHangar = pHangar;
}

void Dashboard::init() {
    takeOffCmdRequested = false;
    landingCmdRequested = false;
    resetAlarmRequested = false;
}

void Dashboard::notifyNewState() {
    // 1. Recupera TUTTI e 4 i dati
    int droneState = pHangar->getDroneStateCode();
    float droneDist = pHangar->getDroneDistance();
    float temp = pHangar->getTemperature();
    int hangarState = pHangar->getHangarStateCode(); // <--- NUOVO DATO

    // 2. Costruisci il messaggio con 4 parti
    // Formato: dh:st:STATO_DRONE:DISTANZA:TEMP:STATO_HANGAR
    String msg = "dh:st:" + 
                 String(droneState) + ":" + 
                 String(droneDist, 1) + ":" + 
                 String(temp, 1) + ":" + 
                 String(hangarState); // <--- AGGIUNTO ALLA FINE

    MsgService.sendMsg(msg);
}

void Dashboard::sync() {
    if (MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        if (msg != NULL) {
            Logger.log("Received msg: " + msg->getContent());

            String content = msg->getContent();

            if (content == "cmd:takeoff") {
                takeOffCmdRequested = true;
            } else if (content == "cmd:landing") {
                landingCmdRequested = true;
            } else if (content == "cmd:reset") {
                resetAlarmRequested = true;
            }

            delete msg;
        }
    }
}

// Metodi per leggere e resettare i comandi
bool Dashboard::checkAndResetTakeOffRequest() {
    bool com = takeOffCmdRequested;
    takeOffCmdRequested = false;
    return com;
}

bool Dashboard::checkAndResetLandingRequest() {
    bool com = landingCmdRequested;
    landingCmdRequested = false;
    return com;
}

bool Dashboard::checkAndResetAlarmReset() {
    bool com = resetAlarmRequested;
    resetAlarmRequested = false;
    return com;
}
