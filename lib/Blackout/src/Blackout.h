#ifndef BLACKOUT_DETECT_O_MATIC_BLACKOUT_H
#define BLACKOUT_DETECT_O_MATIC_BLACKOUT_H

#include <ctime>
#include <ArduinoJson.h>
#include "UnixTimeWithMilliSeconds.h"

#define BLACKOUT_FIELD_START "start"
#define BLACKOUT_FIELD_END "end"
#define BLACKOUT_FIELD_DURATION "duration"

class Blackout {
public:
    Blackout() = default;

    explicit Blackout(UnixTimeWithMilliSeconds startTime);

    Blackout(UnixTimeWithMilliSeconds startTime, UnixTimeWithMilliSeconds endTime);

    explicit Blackout(JsonDocument doc);

    void setStart(UnixTimeWithMilliSeconds startTime);

    void setEnd(UnixTimeWithMilliSeconds endTime);

    UnixTimeWithMilliSeconds getStart();

    UnixTimeWithMilliSeconds getEnd();

    Duration getDuration();

    /**
     * Returns a JsonDocument representation of this object.
     * Trying to create and return a JsonObject did not work, so lets stick with the JsonDocument.
     *
     * @return JsonDocument
     */
    JsonDocument toJsonDocument();

    JsonDocument toHumanReadableJsonDocument();

    bool isActive();

    bool isValid(); // true if start and end are set and they are consistent

    bool equals(Blackout other);

private:
    UnixTimeWithMilliSeconds start = {0, 0};
    UnixTimeWithMilliSeconds end = {0, 0};
};

#endif //BLACKOUT_DETECT_O_MATIC_BLACKOUT_H
