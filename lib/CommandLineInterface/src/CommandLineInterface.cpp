#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "CommandLineInterface.h"

String CommandLineInterface::executeCommand(String command) {
    if (command.equals("ping")) {
        return "pong";
    } else if (command.equals("clock")) {
        return "Clock source: " + rtcAdapter.clockSourceToString(rtcAdapter.getClockSource()) + ", time: " +
               rtcAdapter.getTime().getFormattedTime();
    } else if (command.equals("uptime")) {
        return Duration(1, millis() / 1000, millis() % 1000).getFormattedDuration();
    } else if (command.equals("status")) {
        return getStatus();
    } else if (command.equals("clearAlarm")) {
        serialLogger.info("CommandLineInterface: clearAlarm");
        systemStatus.clearAlarm(rtcAdapter.getTime());
        return "Alarm cleared";
    } else if (command.equals("resetStatus")) {
        serialLogger.info("CommandLineInterface: resetStatus");
        systemStatus.reset(rtcAdapter.getTime());
        return "Status reset";
    } else if (command.equals("help")) {
        return getHelp();
    } else {
        return "unknown command";
    }
}

String CommandLineInterface::getStatus() {
    String result;
    serializeJsonPretty(systemStatus.toHumanReadableJsonDocument(), result);
    return result;
}

String CommandLineInterface::getHelp() {
    return "ping - echo request\n"
           "clock - get the current time\n"
           "uptime - get the uptime\n"
           "status - get the status of the system\n"
           "clearAlarm - clear the alarm\n"
           "resetStatus - reset reboot / blackout counters and clear alarm\n";
}

CommandLineInterface commandLineInterface;
#pragma clang diagnostic pop