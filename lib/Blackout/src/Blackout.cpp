#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

#include "Blackout.h"

Blackout::Blackout(UnixTimeWithMilliSeconds start) {
    setStart(start);
}

Blackout::Blackout(UnixTimeWithMilliSeconds start, UnixTimeWithMilliSeconds end) {
    setStart(start);
    setEnd(end);
}

Blackout::Blackout(JsonDocument doc) {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(doc[BLACKOUT_FIELD_START], doc[BLACKOUT_FIELD_START_MILLIS]);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(doc[BLACKOUT_FIELD_END], doc[BLACKOUT_FIELD_END_MILLIS]);
    setStart(start);
    setEnd(end);
}

void Blackout::setStart(UnixTimeWithMilliSeconds start) {
    this->start = start;
}

UnixTimeWithMilliSeconds Blackout::getStart() {
    return start;
}

void Blackout::setEnd(UnixTimeWithMilliSeconds end) {
    this->end = end;
}

UnixTimeWithMilliSeconds Blackout::getEnd() {
    return end;
}

UnixTimeWithMilliSeconds::Duration Blackout::getDuration() {
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