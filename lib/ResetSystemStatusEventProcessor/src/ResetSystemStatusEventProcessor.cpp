#include "ResetSystemStatusEventProcessor.h"

void ResetSystemStatusEventProcessor::processEvents() {
    if (resetSystemStatusEventBuffer.resetEventCount) {
        systemStatus.reset(rtcAdapter.getTime());
        resetSystemStatusEventBuffer.resetEventCount = 0;
        serialLogger.info("ResetSystemStatusEventProcessor: System status reset");
    }
}

ResetSystemStatusEventProcessor resetSystemStatusEventProcessor;
ResetSystemStatusEventBuffer resetSystemStatusEventBuffer;