#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "PowerChangeEventProcessor.h"

PowerChangeEventProcessor powerChangeEventProcessor = PowerChangeEventProcessor();
PowerChangeEventBuffer powerChangeEventBuffer;

void PowerChangeEventProcessor::processEvents(bool isPowerOnline) {
    logEventStats(isPowerOnline);
    if (isPowerOnline) {
        processEventsWhenPowerIsUp();
    } else {
        processEventsWhenPowerIsDown();
    }
    clearEvents();
}

void PowerChangeEventProcessor::logEventStats(bool isPowerOnline) {
    serialLogger.debug("PowerChangeEventProcessor:"
                       " Registered " + String(powerChangeEventBuffer.powerDownEventCount) + " power down events and "
                       + String(powerChangeEventBuffer.powerUpEventCount) + " power up events since last run."
                                                                            " Power is " +
                       String(isPowerOnline ? "up" : "down") + " at the moment.");
}

void PowerChangeEventProcessor::processEventsWhenPowerIsUp() {
    if (systemStatus.getLastBlackout().isActive()) {
        systemStatus.setPowerStatus(rtcAdapter.getTime(), true);
        serialLogger.info("PowerChangeEventProcessor: Power Up at " +
                          systemStatus.getLastBlackout().getEnd().getFormattedTime());
    } else if (powerChangeEventBuffer.powerUpEventCount || powerChangeEventBuffer.powerDownEventCount) {
        // register momentary blackout
        systemStatus.setPowerStatus(rtcAdapter.getTime(), false);
        systemStatus.setPowerStatus(rtcAdapter.getTime(), true);
        serialLogger.info("PowerChangeEventProcessor: Momentary blackout detected");
    }
}

void PowerChangeEventProcessor::processEventsWhenPowerIsDown() {
    if (systemStatus.getLastBlackout().isActive()) {
        if (powerChangeEventBuffer.powerDownEventCount || powerChangeEventBuffer.powerUpEventCount) {
            serialLogger.info("PowerChangeEventProcessor: power fluctuation detected");
        }
    } else {
        systemStatus.setPowerStatus(rtcAdapter.getTime(), false);
        serialLogger.info("PowerChangeEventProcessor: Power Down at " + rtcAdapter.getTime().getFormattedTime());
    }
}

void PowerChangeEventProcessor::clearEvents() {
    powerChangeEventBuffer.powerDownEventCount = 0;
    powerChangeEventBuffer.powerUpEventCount = 0;
}

#pragma clang diagnostic pop