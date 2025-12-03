#ifndef TAKEOFFTASK_H
#define TAKEOFFTASK_H

#include <Arduino.h>
#include "task/Task.h"
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
        TAKING_OFF,
        COMPLETED
    };
    DroneHangar* hangar;
    State state;
    unsigned long stateStartTime;
    bool justEntered;

    void setState(State newState);
    void blinkL2();
    bool checkAndSetJustEntered();
    unsigned long elapsedTimeInState();
    
};