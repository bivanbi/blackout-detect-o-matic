#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "CommandLineInterface.h"

String CommandLineInterface::executeCommand(String commandLine) {
    commandLine.trim();
    CommandAndArguments cmd = splitCommandAndArguments(commandLine);

    if (cmd.command.equals("ping")) {
        return "pong";
    } else if (cmd.command.equals("clock")) {
        return "Clock source: " + rtcAdapter.clockSourceToString(rtcAdapter.getClockSource()) + ", time: " +
               rtcAdapter.getTime().getFormattedTime();
    } else if (cmd.command.equals("uptime")) {
        return Duration(1, millis() / 1000, millis() % 1000).getFormattedDuration();
    } else if (cmd.command.equals("status")) {
        return getStatus();
    } else if (cmd.command.equals("clearAlarm")) {
        serialLogger.info("CommandLineInterface: clearAlarm");
        systemStatus.clearAlarm(rtcAdapter.getTime());
        return "Alarm cleared";
    } else if (cmd.command.equals("resetStatus")) {
        serialLogger.info("CommandLineInterface: resetStatus");
        systemStatus.reset(rtcAdapter.getTime());
        return "Status reset";
    } else if (cmd.command.equals("saveStatus")) {
        serialLogger.info("CommandLineInterface: saveStatus");
        if (SystemStatusLoader::save()) {
            return "Status saved";
        } else {
            return "Failed to save status";
        }
    } else if (cmd.command.equals(CLI_COMMAND_CONFIG)) {
        return config.executeCommand(cmd.arguments);
    } else if (cmd.command.isEmpty() || cmd.command.equals("help")) {
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
           CLI_COMMAND_CONFIG " - configuration commands, issue 'config help' for more info"
           "uptime - get the uptime\n"
           "status - get the status of the system\n"
           "clearAlarm - clear the alarm\n"
           "resetStatus - reset reboot / blackout counters and clear alarm\n"
           "saveStatus - save status to persistent storage (SD card)\n";
}

CommandLineInterface::CommandAndArguments CommandLineInterface::splitCommandAndArguments(String commandLine) {
    String arguments;
    int spaceIndex = commandLine.indexOf(' ');
    String command = commandLine.substring(0, spaceIndex);
    if (spaceIndex > 0) {
        arguments = commandLine.substring(spaceIndex + 1);
    }

    return {command, arguments};
}

CommandLineInterface commandLineInterface;

String CommandLineInterface::ConfigCLI::executeCommand(String commandLine) {
    CommandAndArguments configCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    serialLogger.debug("CommandLineInterface::Config::executeCommand:'" + configCommand.command + "', args: '" + configCommand.arguments + "'");

    if (configCommand.command.equals(CLI_COMMAND_CONFIG_GET)) {
        return getConfig();
    } else if (configCommand.command.equals("") || configCommand.command.equals("help")) {
        return getHelp();
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::ConfigCLI::getConfig() {
    String result;
    serializeJsonPretty(configuration.toJsonDocument(), result);
    return result;
}

String CommandLineInterface::ConfigCLI::getHelp() {
    return "config get - get the current configuration\n";
}

#pragma clang diagnostic pop
