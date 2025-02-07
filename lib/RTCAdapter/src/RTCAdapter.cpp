#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#include "RTCAdapter.h"

RTCAdapter::RTCAdapter(int offset) {
    id = ID++;
    this->offset = offset;
    clockSource = ClockSource::NONE;
    rtc = ESP32Time(offset);
}

void RTCAdapter::setTime(RTCAdapter::ClockSource source, UnixTimeWithMilliSeconds time) {
    int microSeconds = (int) time.getMilliSeconds() * 1000;
    rtc.setTime(time.getUnixTime(), microSeconds); // ms is a lie. it is us
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

String RTCAdapter::clockSourceToString(RTCAdapter::ClockSource source) {
    String sourceString = "Unknown";

    switch (source) {
        case ClockSource::NONE:
            sourceString = "None";
            break;

        case ClockSource::NTP:
            sourceString = "NTP";
            break;

        case ClockSource::FILE_DATE:
            sourceString = "File Date";
            break;
    }

    return sourceString + " (" + String(source) + ")";
}

RTCAdapter rtcAdapter = RTCAdapter();

#pragma clang diagnostic pop