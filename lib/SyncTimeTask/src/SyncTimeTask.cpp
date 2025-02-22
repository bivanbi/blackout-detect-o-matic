#include "SyncTimeTask.h"

void SyncTimeTask::syncTime() {
    ntpClientAdapter.update();
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, ntpClientAdapter.getUnixTime());
    Logger::debug("Time updated: " + ntpClientAdapter.getUnixTime().getFormattedTime());
}