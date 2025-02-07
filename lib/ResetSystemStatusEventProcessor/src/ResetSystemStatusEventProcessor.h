#ifndef BLACKOUT_DETECT_O_MATIC_RESET_SYSTEM_STATUS_EVENT_PROCESSOR_H
#define BLACKOUT_DETECT_O_MATIC_RESET_SYSTEM_STATUS_EVENT_PROCESSOR_H

#include <Arduino.h>
#include "SystemStatus.h"
#include "RTCAdapter.h"
#include "SerialLogger.h"

struct ResetSystemStatusEventBuffer {
    unsigned int resetEventCount = 0;
};

class ResetSystemStatusEventProcessor {
public:
    ResetSystemStatusEventProcessor() = default;

    void processEvents();
};

extern ResetSystemStatusEventProcessor resetSystemStatusEventProcessor;
extern ResetSystemStatusEventBuffer resetSystemStatusEventBuffer;

#endif //BLACKOUT_DETECT_O_MATIC_RESET_SYSTEM_STATUS_EVENT_PROCESSOR_H
