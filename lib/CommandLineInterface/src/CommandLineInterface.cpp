#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "CommandLineInterface.h"

String CommandLineInterface::executeCommand(String commandLine) {
    commandLine.trim();
    CommandAndArguments cmd = splitCommandAndArguments(commandLine);

    if (cmd.command.equals(CLI_COMMAND_ALARM)) {
        return alarm.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_CONFIG)) {
        return config.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_DATE)) {
        return "Clock source: " + rtcAdapter.clockSourceToString(rtcAdapter.getClockSource()) + ", time: " +
               rtcAdapter.getTime().getFormattedTime();
    } else if (cmd.command.equals(CLI_COMMAND_HELP)) {
        return getHelp();
    } else if (cmd.command.equals(CLI_COMMAND_MEMINFO)) {
        return MemoryInfo::getFormattedMemoryInfo();
    } else if (cmd.command.equals(CLI_COMMAND_PING)) {
        return "pong";
    } else if (cmd.command.equals(CLI_COMMAND_REBOOT)) {
        return scheduleReboot(cmd);
    } else if (cmd.command.equals(CLI_COMMAND_STATUS)) {
        return status.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_UPTIME)) {
        return getUptime();
    } else if (cmd.command.isEmpty()) {
        return "";
    } else {
        return "unknown command";
    }
}

String CommandLineInterface::getUptime() {
    return rtcAdapter.getTime().getFormattedTime() + " up "
           + UptimeAdapter::getFormattedUptime();
}

String CommandLineInterface::scheduleReboot(CommandLineInterface::CommandAndArguments cmd) {
        if (cmd.arguments.isEmpty()) {
            cmd.arguments = String(CLI_DEFAULT_REBOOT_DELAY);
        }

        PeriodicTaskScheduler::scheduleReboot(cmd.arguments.toInt());
        return "Rebooting in " + String(CLI_DEFAULT_REBOOT_DELAY) + " seconds";
}

String CommandLineInterface::getHelp() {
    return "reboot <delay> - reboot the system with optional delay in seconds, default: " + String(CLI_DEFAULT_REBOOT_DELAY) + " seconds\n"
           "ping - echo request\n"
           "clock - get the current time\n"
           CLI_COMMAND_CONFIG " - configuration commands, issue 'config help' for more info\n"
           CLI_COMMAND_MEMINFO " - get memory usage info\n"
           "uptime - get the uptime\n"
           CLI_COMMAND_STATUS " - status commands, issue 'status help' for more info\n"
           "clearAlarm - clear the alarm\n";
}

String CommandLineInterface::AlarmCLI::executeCommand(String commandLine) {
    CommandAndArguments alarmCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    serialLogger.debug("CommandLineInterface::Alarm::executeCommand:'" + alarmCommand.command + "', args: '" +
                       alarmCommand.arguments + "'");

    if (alarmCommand.command.equals(CLI_COMMAND_ALARM_CLEAR)) {
        return clearAlarm();
    } else if (alarmCommand.command.equals(CLI_COMMAND_HELP)) {
        return getHelp();
    } else if (alarmCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::AlarmCLI::clearAlarm() {
    systemStatus.clearAlarm(rtcAdapter.getTime());
    return "Alarm cleared";
}

String CommandLineInterface::AlarmCLI::getHelp() {
    return "alarm clear - clear the alarm\n";
}

String CommandLineInterface::ConfigCLI::executeCommand(String commandLine) {
    CommandAndArguments configCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    serialLogger.debug("CommandLineInterface::Config::executeCommand:'" + configCommand.command + "', args: '" +
                       configCommand.arguments + "'");

    if (configCommand.command.equals(CLI_COMMAND_CONFIG_GET)) {
        return getConfig(configCommand.arguments);
    } else if (configCommand.command.equals(CLI_COMMAND_CONFIG_SET)) {
        return setConfig(configCommand.arguments);
    } else if (configCommand.command.equals(CLI_COMMAND_CONFIG_SAVE)) {
        return saveConfig();
    } else if (configCommand.command.equals("help")) {
        return getHelp();
    } else if (configCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::ConfigCLI::getConfig(String key) {
    if (key.isEmpty()) {
        return getConfig();
    }

    Configuration::GetResult result = configuration.get(key);
    if (result.code == Configuration::ResultCode::OK) {
        return result.value;
    }

    serialLogger.error(
            "CommandLineInterface::ConfigCLI::getConfig: key: '" + key + "', error message: " + result.value);
    return "ERROR: unknown key";
}

String CommandLineInterface::ConfigCLI::getConfig() {
    String result;
    serializeJsonPretty(configuration.toJsonDocument(), result);
    return result;
}

String CommandLineInterface::ConfigCLI::setConfig(String keyAndValue) {
    KeyValue keyValuePair = commandLineInterface.splitKeyValuePair(keyAndValue);
    if (keyValuePair.key.isEmpty()) {
        return "ERROR: expected key=value pair";
    }

    Configuration::SetResult result = configuration.set(keyValuePair.key, keyValuePair.value);
    if (result.code == Configuration::ResultCode::OK) {
        return "Configuration updated: " + keyAndValue;
    }

    serialLogger.error("CommandLineInterface::ConfigCLI::setConfig: "
                       "key: '" + keyValuePair.key + "', value: '" + keyValuePair.value +
                       "', error message: " + result.message);
    return "ERROR: " + result.message;
}

String CommandLineInterface::ConfigCLI::saveConfig() {
    if (ConfigurationLoader::save()) {
        return "Configuration saved";
    }

    return "Failed to save configuration";
}

String CommandLineInterface::ConfigCLI::getHelp() {
    return "config get - get the current configuration\n"
           "config get <key> - get the value of a specific key\n"
           "config set <key>=<value> - set the value of a specific key\n"
           "config save - save the configuration to persistent storage (SD card)\n\n"
           "Note: reboot to apply the saved configuration\n";
}

String CommandLineInterface::StatusCLI::executeCommand(String commandLine) {
    CommandAndArguments statusCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    serialLogger.debug("CommandLineInterface::Status::executeCommand:'" + statusCommand.command + "', args: '" +
                       statusCommand.arguments + "'");

    if (statusCommand.command.equals(CLI_COMMAND_STATUS_GET)) {
        return getStatus();
    } else if (statusCommand.command.equals(CLI_COMMAND_STATUS_RESET)) {
        return resetStatus();
    } else if (statusCommand.command.equals(CLI_COMMAND_STATUS_SAVE)) {
        return saveStatus();
    } else if (statusCommand.command.equals(CLI_COMMAND_HELP)) {
        return getHelp();
    } else if (statusCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::StatusCLI::getStatus() {
    String result;
    serializeJsonPretty(systemStatus.toHumanReadableJsonDocument(), result);
    return result;
}

String CommandLineInterface::StatusCLI::resetStatus() {
    systemStatus.reset(rtcAdapter.getTime());
    return "Status reset";
}

String CommandLineInterface::StatusCLI::saveStatus() {
    if (SystemStatusLoader::save()) {
        return "Status saved";
    }

    return "Failed to save status";
}

String CommandLineInterface::StatusCLI::getHelp() {
    return "status get - get the current status\n"
           "status reset - reset the status counters\n"
           "status save - save the status to persistent storage (SD card)\n";
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

CommandLineInterface::KeyValue CommandLineInterface::splitKeyValuePair(String keyAndValue) {
    int equalsIndex = keyAndValue.indexOf('=');
    if (equalsIndex < 0) {
        return {"", ""};
    }

    return {keyAndValue.substring(0, equalsIndex), keyAndValue.substring(equalsIndex + 1)};
}

CommandLineInterface commandLineInterface;

#pragma clang diagnostic pop
