#ifndef BLACKOUT_DETECT_O_MATIC_BLACKOUT_H
#define BLACKOUT_DETECT_O_MATIC_BLACKOUT_H

#include <ctime>
#include <ArduinoJson.h>
#include "UnixTimeWithMilliSeconds.h"

#define BLACKOUT_FIELD_START "start"
#define BLACKOUT_FIELD_START_MILLIS "startMillis"
#define BLACKOUT_FIELD_END "end"
#define BLACKOUT_FIELD_END_MILLIS "endMillis"

class Blackout {
public:
    Blackout() = default;

    explicit Blackout(UnixTimeWithMilliSeconds start);

    Blackout(UnixTimeWithMilliSeconds start, UnixTimeWithMilliSeconds end);

    explicit Blackout(JsonDocument doc);

    void setStart(UnixTimeWithMilliSeconds start);

    void setEnd(UnixTimeWithMilliSeconds end);

    UnixTimeWithMilliSeconds getStart();

    UnixTimeWithMilliSeconds getEnd();

    UnixTimeWithMilliSeconds::Duration getDuration();

    JsonDocument toJsonDocument();

    bool isActive();

    bool isValid(); // true if start and end are set and they are consistent

private:
    UnixTimeWithMilliSeconds start = {0, 0};
    UnixTimeWithMilliSeconds end = {0, 0};
};

#endif //BLACKOUT_DETECT_O_MATIC_BLACKOUT_H
