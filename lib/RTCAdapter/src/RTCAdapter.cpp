#include "RTCAdapter.h"

RTCAdapter::RTCAdapter(int offset) {
    id = ID++;
    this->offset = offset;
    clockSource = ClockSource::NONE;
    rtc = ESP32Time(offset);
}

void RTCAdapter::setTime(RTCAdapter::ClockSource source, unsigned long unixTime, int millis) {
    int microSeconds = millis * 1000;
    rtc.setTime(unixTime, microSeconds); // ms is a lie. it is us
    clockSource = source;
}

UnixTimeWithMilliSeconds RTCAdapter::getTime() {
    return {rtc.getEpoch(), rtc.getMillis()};
}

int RTCAdapter::getOffset() const {
    return offset;
}

RTCAdapter::ClockSource RTCAdapter::getClockSource() {
    return clockSource;
}

unsigned long RTCAdapter::getMicros() {
    return rtc.getMicros();
}

int RTCAdapter::ID = 0;

bool RTCAdapter::isTimeSet() {
    return clockSource == ClockSource::NTP || clockSource == ClockSource::FILE_DATE;
}

RTCAdapter rtcAdapter = RTCAdapter();
