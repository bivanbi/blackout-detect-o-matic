#include "ClearAlarmEventProcessor.h"

ClearAlarmEventBuffer clearAlarmEventBuffer;
ClearAlarmEventProcessor clearAlarmEventProcessor = ClearAlarmEventProcessor();

void ClearAlarmEventProcessor::processEvents() {
    if (clearAlarmEventBuffer.clearAlarmEventCount) {
        if (systemStatus.isAlarmActive()) {
            Logger::info("ClearAlarmEventProcessor: Clearing alarm");
            systemStatus.clearAlarm(rtcAdapter.getTime());
        } else {
            Logger::debug("ClearAlarmEventProcessor: clear alarm event received, but alarm is not active");
        }
        clearAlarmEventBuffer.clearAlarmEventCount = 0;
    }
}
