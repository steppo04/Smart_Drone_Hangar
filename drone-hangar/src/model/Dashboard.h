#ifndef __DASHBOARD__
#define __DASHBOARD__

#include "config.h"
#include "HWPlatform.h"
#include "model/DroneHangar.h"

class Dashboard {
    public:
        Dashboard(DroneHangar* pHangar);

        void init();
        
        void notifyNewState();
        
        bool checkAndResetTakeOffRequest();
        bool checkAndResetLandingRequest();
        bool checkAndResetAlarmReset();

        void sync();

    private:
        DroneHangar* pHangar;
        bool takeOffCmdRequested;
        bool landingCmdRequested; 
        bool resetAlarmRequested;
    };

#endif