#include "SyncTimeTask.h"

void SyncTimeTask::syncTime() {
    ntpClientAdapter.update();
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, ntpClientAdapter.getUnixTime());
    serialLogger.debug("Time updated: " + ntpClientAdapter.getUnixTime().getFormattedTime());
}