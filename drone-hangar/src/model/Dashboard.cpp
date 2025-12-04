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
    String st;
    if (pHangar->isHangarAlarmed()) {
        st = "2";            // ALARM
    } else if (pHangar->isHangarPreAlarm()) {
        st = "1";            // PRE-ALARM
    } else {
        st = "0";            // OK
    }

    float droneDist = pHangar->getDroneDistance();
    float temp = pHangar->getTemperature();

    MsgService.sendMsg(String("dh:st:") + st + ":" +
                       String(droneDist).substring(0,5) + ":" +
                       String(temp).substring(0,5));
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
