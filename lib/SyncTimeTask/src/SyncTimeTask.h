#ifndef BLACKOUT_DETECT_O_MATIC_SYNC_TIME_TASK_H
#define BLACKOUT_DETECT_O_MATIC_SYNC_TIME_TASK_H

#include <Arduino.h>

#include "SerialLogger.h"
#include "RTCAdapter.h"
#include "NTPClientAdapter.h"

class SyncTimeTask {
public:
    static void syncTime();
};


#endif //BLACKOUT_DETECT_O_MATIC_SYNC_TIME_TASK_H
