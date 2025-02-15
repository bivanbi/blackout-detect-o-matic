#ifndef BLACKOUT_DETECT_O_MATIC_PERSISTENT_LOGGER_H
#define BLACKOUT_DETECT_O_MATIC_PERSISTENT_LOGGER_H

#include <Arduino.h>

#include "Configuration.h"
#include "LogHelper.h"
#include "LogSeverity.h"
#include "RTCAdapter.h"
#include "PersistentStorage.h"

class PersistentLogger {
public:
    static void begin();

    static void setLogLevel(LogSeverity::Severity level);

    static void log(LogSeverity::Severity severity, const String& message);

    static String logFilePath;
    static LogSeverity::Severity logLevel;
private:
};

#endif //BLACKOUT_DETECT_O_MATIC_PERSISTENT_LOGGER_H
