#ifndef __LANDING_TASK__
#define __LANDING_TASK__

#include "kernel/Task.h"
#include "model/DroneHangar.h"
#include "model/UserPanel.h"
#include <model/Dashboard.h>

class LandingTask : public Task {

public:
    LandingTask(DroneHangar* hangar, UserPanel* panel, Dashboard* dashboard);
    void tick();

private:
    enum State { 
        WAITING_FOR_LANDING, 
        DOOR_OPENING, 
        LANDING, 
        TIMING,
        DOOR_CLOSING, 
        DRONE_INSIDE 
    };

    void setState(State state);
    unsigned long elapsedTimeInState();
    bool checkAndSetJustEntered();

    State state;
    DroneHangar* hangar;
    Dashboard* dashboard;
    UserPanel* pPanel;
    unsigned long stateStartTime;
    bool justEntered;

};

#endif