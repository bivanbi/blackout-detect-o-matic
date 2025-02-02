#ifndef BLACKOUT_DETECT_O_MATIC_CLEAR_ALARM_EVENT_PROCESSOR_H
#define BLACKOUT_DETECT_O_MATIC_CLEAR_ALARM_EVENT_PROCESSOR_H

#include <Arduino.h>

#include "RTCAdapter.h"
#include "SystemStatus.h"
#include "SerialLogger.h"

struct ClearAlarmEventBuffer {
    unsigned long clearAlarmEventCount = 0;
};

class ClearAlarmEventProcessor {
public:
    ClearAlarmEventProcessor() = default;

    void processEvents();

};

extern ClearAlarmEventProcessor clearAlarmEventProcessor;
extern ClearAlarmEventBuffer clearAlarmEventBuffer;

#endif //BLACKOUT_DETECT_O_MATIC_CLEAR_ALARM_EVENT_PROCESSOR_H
