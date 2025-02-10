#ifndef BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H
#define BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H

#include <sys/_stdint.h>

#include "UnixTimeWithMilliSeconds.h"
#include "RTCAdapter.h"

#define SERIAL_LOGGER_BAUD_RATE 115200

class SerialLogger {
public:
    enum LogLevel {
        NONE = 0,
        ERROR = 1,
        INFO = 2,
        DEBUG = 3
    };

    SerialLogger();

    void setLogLevel(String level);

    void setLogLevel(LogLevel level);

    LogLevel stringToLogLevel(String level);

    String logLevelToString(LogLevel level);

    void debug(String message);

    void info(String message);

    void error(String message);

private:
    String getLogHeader(String level);

    void log(String level, String message);

    LogLevel logLevel = LogLevel::INFO;
};

extern SerialLogger serialLogger;

#endif // BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H
