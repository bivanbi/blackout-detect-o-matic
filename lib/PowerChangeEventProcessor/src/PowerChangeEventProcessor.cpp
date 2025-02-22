#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "PowerChangeEventProcessor.h"

PowerChangeEventProcessor powerChangeEventProcessor = PowerChangeEventProcessor();
PowerChangeEventBuffer powerChangeEventBuffer;

const String PowerChangeEventProcessor::logTag = "PowerChangeEventProcessor: ";

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
    Logger::debug(logTag
                  + "Registered " + String(powerChangeEventBuffer.powerDownEventCount) + " power down events and "
                  + String(powerChangeEventBuffer.powerUpEventCount) + " power up events since last run."
                  + " Power is " + String(isPowerOnline ? "up" : "down") + " at the moment.");
}

void PowerChangeEventProcessor::processEventsWhenPowerIsUp() {
    if (systemStatus.getLastBlackout().isActive()) {
        systemStatus.setPowerStatus(rtcAdapter.getTime(), true);
        Logger::info(logTag + "Power Up");
    } else if (powerChangeEventBuffer.powerUpEventCount || powerChangeEventBuffer.powerDownEventCount) {
        // register momentary blackout
        systemStatus.setPowerStatus(rtcAdapter.getTime(), false);
        systemStatus.setPowerStatus(rtcAdapter.getTime(), true);
        Logger::info(logTag + "Momentary blackout detected");
    }
}

void PowerChangeEventProcessor::processEventsWhenPowerIsDown() {
    if (systemStatus.getLastBlackout().isActive()) {
        if (powerChangeEventBuffer.powerDownEventCount || powerChangeEventBuffer.powerUpEventCount) {
            Logger::info(logTag + "power fluctuation detected");
        }
    } else {
        systemStatus.setPowerStatus(rtcAdapter.getTime(), false);
        Logger::info(logTag + "Power Down");
    }
}

void PowerChangeEventProcessor::clearEvents() {
    powerChangeEventBuffer.powerDownEventCount = 0;
    powerChangeEventBuffer.powerUpEventCount = 0;
}

#pragma clang diagnostic pop