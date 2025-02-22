#ifndef BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H
#define BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "Duration.h"

#define UNIX_TIME_FIELD_SECONDS "unixTime"
#define UNIX_TIME_FIELD_MILLI_SECONDS "milliSeconds"

class UnixTimeWithMilliSeconds {
public:
    UnixTimeWithMilliSeconds() = default;

    explicit UnixTimeWithMilliSeconds(unsigned long unixTime);

    UnixTimeWithMilliSeconds(unsigned long unixTime, unsigned int milliSeconds);

    explicit UnixTimeWithMilliSeconds(JsonObject doc);

    unsigned long getUnixTime();

    unsigned int getMilliSeconds();

    String getFormattedTime(const String &format = "%Y-%m-%d %H:%M:%S");

    String getMillisAsZeroPaddedString();

    bool equals(UnixTimeWithMilliSeconds other);

    bool greaterThan(UnixTimeWithMilliSeconds other);

    bool greaterOrEqual(UnixTimeWithMilliSeconds other);

    bool lessThan(UnixTimeWithMilliSeconds other);

    Duration getDuration(UnixTimeWithMilliSeconds other);

    /**
     * Returns a JsonDocument representation of this object.
     * Trying to create and return a JsonObject did not work, so lets stick with the JsonDocument.
     *
     * @return JsonDocument
     */
    JsonDocument toJsonDocument();

private:
    unsigned long unixTime = 0;
    unsigned int milliSeconds = 0;
};

#endif //BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H
