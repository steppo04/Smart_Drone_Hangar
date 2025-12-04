#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "model/UserPanel.h"
#include "model/Dashboard.h"
#include "model/DroneHangar.h"
#include "task/TakeOffTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
UserPanel* pUserPanel;
DroneHangar* pDroneHangar;
Dashboard* pDashboard; 

void setup() {
  MsgService.init();
  sched.init(100);

  Logger.log(":::::: Drone Hangar ::::::");
  
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  pUserPanel = new UserPanel(pHWPlatform);
  pUserPanel->init();

  pDroneHangar = new DroneHangar(pHWPlatform);
  pDroneHangar->init();

  pDashboard = new Dashboard(pDroneHangar);
  pDashboard->init();

  Task* pTakeOffTask = new TakeOffTask(pDroneHangar, pUserPanel, pDashboard);
  pTakeOffTask->init(50);

  sched.addTask(pTakeOffTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task* pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(1000);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
    sched.schedule();
}