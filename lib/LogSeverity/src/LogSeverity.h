#ifndef BLACKOUT_DETECT_O_MATIC_LOG_SEVERITY_H
#define BLACKOUT_DETECT_O_MATIC_LOG_SEVERITY_H

#include <Arduino.h>

class LogSeverity {
public:
    enum Severity {
        NONE = 0,
        ERROR = 1,
        INFO = 2,
        DEBUG = 3
    };

    static LogSeverity::Severity stringToSeverity(String level);

    static String severityToString(LogSeverity::Severity level);
};


#endif //BLACKOUT_DETECT_O_MATIC_LOG_SEVERITY_H
