#include "LogHelper.h"

String LogHelper::getLogHeader(LogSeverity::Severity level) {
    UnixTimeWithMilliSeconds time = rtcAdapter.getTime();
    return time.getFormattedTime() + "." + time.getMillisAsZeroPaddedString() + " [" + LogSeverity::severityToString(level) + "] ";
}

