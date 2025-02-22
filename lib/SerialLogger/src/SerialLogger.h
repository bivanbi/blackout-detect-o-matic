#ifndef BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H
#define BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H

#include <Arduino.h>
#include <sys/_stdint.h>

#include "LogSeverity.h"
#include "LogHelper.h"
#include "UnixTimeWithMilliSeconds.h"
#include "RTCAdapter.h"

#define SERIAL_LOGGER_BAUD_RATE 115200

class SerialLogger {
public:
    static void begin();

    static void setLogLevel(LogSeverity::Severity level);

    static void log(LogSeverity::Severity severity, String message);

private:
    static LogSeverity::Severity logLevel;
};

#endif // BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H
