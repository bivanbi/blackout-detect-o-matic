#ifndef BLACKOUT_DETECT_O_MATIC_POWER_CHANGE_EVENT_PROCESSOR_H
#define BLACKOUT_DETECT_O_MATIC_POWER_CHANGE_EVENT_PROCESSOR_H

#include <Arduino.h>

#include "RTCAdapter.h"
#include "SystemStatus.h"
#include "Logger.h"

struct PowerChangeEventBuffer {
    unsigned int powerDownEventCount = 0;
    unsigned int powerUpEventCount = 0;
};

class PowerChangeEventProcessor {
public:
    PowerChangeEventProcessor() = default;

    void processEvents(bool isPowerOnline);

private:
    void logEventStats(bool isPowerOnline);

    void processEventsWhenPowerIsDown();

    void processEventsWhenPowerIsUp();

    void clearEvents();
};

extern PowerChangeEventProcessor powerChangeEventProcessor;
extern PowerChangeEventBuffer powerChangeEventBuffer;

#endif //BLACKOUT_DETECT_O_MATIC_POWER_CHANGE_EVENT_PROCESSOR_H
