#ifndef TAKEOFFTASK_H
#define TAKEOFFTASK_H

#include <Arduino.h>
#include "kernel/Task.h"
#include "model/DroneHangar.h"
#include "model/UserPanel.h"
#include "model/Dashboard.h"

class TakeOffTask : public Task {
  public:
    TakeOffTask(DroneHangar* hangar, UserPanel* panel, Dashboard* dashboard);
    void tick();

  private:
    enum State {
        IDLE,
        PREPARING_TAKEOFF,
        OPENING_DOOR,
        TAKING_OFF,
        TIMING,
        CLOSING_DOOR,
        DRONE_OUT
    };
    DroneHangar* hangar;
    UserPanel* panel;
    Dashboard* dashboard;
    State state;
    unsigned long stateStartTime;
    bool justEntered;

    void setState(State newState);
    bool checkAndSetJustEntered();
    unsigned long elapsedTimeInState();
    
};
#endif // TAKEOFFTASK_H