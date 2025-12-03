#ifndef __LANDING_TASK__
#define __LANDING_TASK__

#include "kernel/Task.h"
#include "model/DroneHangar.h"
#include "model/UserPanel.h"

class LandingTask : public Task {

public:
    LandingTask(DroneHangar* hangar, UserPanel* panel);
    void tick();

private:
    void setState(int state);
    long elapsedTimeInState();
    void log(const String& msg);
    bool checkAndSetJustEntered();

    enum { 
        WAITING_FOR_LANDING, 
        DOOR_OPENING, 
        LANDING, 
        DOOR_CLOSING, 
        DRONE_INSIDE 
    } state;

    long stateTimestamp;
    bool justEntered;

    DroneHangar* pDroneHangar;
    UserPanel* pPanel;

    unsigned long t2StartTime;  // Timestamp di quando il drone entra nel raggio D2
    bool t2TimerActive;         // Flag per sapere se stiamo contando il tempo T2

};

#endif