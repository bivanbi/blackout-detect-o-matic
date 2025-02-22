#include "LogSeverity.h"

LogSeverity::Severity LogSeverity::stringToSeverity(String level) {
    level.toUpperCase();
    if (level == "NONE") {
        return NONE;
    } else if (level == "ERROR") {
        return ERROR;
    } else if (level == "INFO") {
        return INFO;
    } else {
        return DEBUG;
    }
}

String LogSeverity::severityToString(LogSeverity::Severity level) {
    switch(level) {
        case NONE:
            return "NONE";
        case ERROR:
            return "ERROR";
        case INFO:
            return "INFO";
        default:
            return "DEBUG";
    }
}

