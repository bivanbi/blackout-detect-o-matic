#ifndef BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H
#define BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H

#include <Arduino.h>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "Duration.h"

class UnixTimeWithMilliSeconds {
public:
    UnixTimeWithMilliSeconds(time_t unixTime, unsigned int milliSeconds);

    unsigned long getUnixTime();

    unsigned int getMilliSeconds();

    String getFormattedTime(const String &format = "%Y-%m-%d %H:%M:%S");

    String getMillisAsZeroPaddedString();

    bool equals(UnixTimeWithMilliSeconds other);

    bool greaterThan(UnixTimeWithMilliSeconds other);

    bool greaterOrEqual(UnixTimeWithMilliSeconds other);

    bool lessThan(UnixTimeWithMilliSeconds other);

    Duration getDuration(UnixTimeWithMilliSeconds other);

private:
    time_t unixTime;
    unsigned int milliSeconds;
};


#endif //BLACKOUT_DETECT_O_MATIC_UNIX_TIME_WITH_MILLISECONDS_H
