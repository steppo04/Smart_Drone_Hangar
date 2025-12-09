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
        PREPARING,
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
    void blinkL2();
    bool checkAndSetJustEntered();
    unsigned long elapsedTimeInState();
    void log(const String& msg);
    
};
#endif // TAKEOFFTASK_H