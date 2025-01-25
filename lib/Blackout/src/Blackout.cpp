#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

#include "Blackout.h"

Blackout::Blackout(UnixTimeWithMilliSeconds startTime) {
    setStart(startTime);
}

Blackout::Blackout(UnixTimeWithMilliSeconds startTime, UnixTimeWithMilliSeconds endTime) {
    setStart(startTime);
    setEnd(endTime);
}

Blackout::Blackout(JsonDocument doc) {
    setStart(UnixTimeWithMilliSeconds(doc[BLACKOUT_FIELD_START], doc[BLACKOUT_FIELD_START_MILLIS]));
    setEnd(UnixTimeWithMilliSeconds(doc[BLACKOUT_FIELD_END], doc[BLACKOUT_FIELD_END_MILLIS]));
}

void Blackout::setStart(UnixTimeWithMilliSeconds startTime) {
    start = startTime;
}

UnixTimeWithMilliSeconds Blackout::getStart() {
    return start;
}

void Blackout::setEnd(UnixTimeWithMilliSeconds endTime) {
    end = endTime;
}

UnixTimeWithMilliSeconds Blackout::getEnd() {
    return end;
}

Duration Blackout::getDuration() {
    return end.getDuration(start);
}

bool Blackout::isActive() {
    return start.getUnixTime() > 0 && end.getUnixTime() == 0;
}

bool Blackout::isValid() {
    return start.getUnixTime() > 0 && end.getUnixTime() > 0 && end.greaterOrEqual(start);
}

JsonDocument Blackout::toJsonDocument() {
    JsonDocument doc;
    doc[BLACKOUT_FIELD_START] = start.getUnixTime();
    doc[BLACKOUT_FIELD_START_MILLIS] = start.getMilliSeconds();
    doc[BLACKOUT_FIELD_END] = end.getUnixTime();
    doc[BLACKOUT_FIELD_END_MILLIS] = end.getMilliSeconds();

    return doc;
}

#pragma clang diagnostic pop