#ifndef BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H
#define BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H

#include <sys/_stdint.h>

#include "UnixTimeWithMilliSeconds.h"
#include "RTCAdapter.h"

#define SERIAL_LOGGER_BAUD_RATE 115200

class SerialLogger {
public:
    SerialLogger();

    void debug(String message);

    void info(String message);

    void error(String message);

private:
    String getLogHeader(String level);

    void log(String level, String message);
};

extern SerialLogger serialLogger;

#endif // BLACKOUT_DETECT_O_MATIC_SERIAL_LOGGER_H
