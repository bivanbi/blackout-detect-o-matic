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

    explicit UnixTimeWithMilliSeconds(time_t unixTime, unsigned int milliSeconds = 0);

    explicit UnixTimeWithMilliSeconds(JsonDocument doc);

    unsigned long getUnixTime();

    unsigned int getMilliSeconds();

    String getFormattedTime(const String &format = "%Y-%m-%d %H:%M:%S");

    String getMillisAsZeroPaddedString();

    bool equals(UnixTimeWithMilliSeconds other);

    bool greaterThan(UnixTimeWithMilliSeconds other);

    bool greaterOrEqual(UnixTimeWithMilliSeconds other);

    bool lessThan(UnixTimeWithMilliSeconds other);

    Duration getDuration(UnixTimeWithMilliSeconds other);

    JsonDocument toJsonDocument();

private:
    time_t unixTime = 0;
    unsigned int milliSeconds = 0;
};

#endif //BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H
