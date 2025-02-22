#ifndef BLACKOUT_DETECT_O_MATIC_LOGGER_ENUM_H
#define BLACKOUT_DETECT_O_MATIC_LOGGER_ENUM_H

#include <Arduino.h>

#include "UnixTimeWithMilliSeconds.h"
#include "RTCAdapter.h"
#include "LogSeverity.h"

class LogHelper {
public:
    static String getLogHeader(LogSeverity::Severity severity);
};

#endif //BLACKOUT_DETECT_O_MATIC_LOGGER_ENUM_H
