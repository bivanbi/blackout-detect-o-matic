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
    setStart(UnixTimeWithMilliSeconds(doc[BLACKOUT_FIELD_START].as<JsonObject>()));
    setEnd(UnixTimeWithMilliSeconds(doc[BLACKOUT_FIELD_END].as<JsonObject>()));
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
    doc[BLACKOUT_FIELD_START] = start.toJsonDocument();
    doc[BLACKOUT_FIELD_END] = end.toJsonDocument();

    return doc;
}

bool Blackout::equals(Blackout other) {
    return start.equals(other.getStart()) && end.equals(other.getEnd());
}

#pragma clang diagnostic pop
