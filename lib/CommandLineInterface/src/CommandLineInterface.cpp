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
    } else if (cmd.command.isEmpty() || cmd.command.equals("help") || cmd.command.equals("?")) {
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
           CLI_COMMAND_CONFIG " - configuration commands, issue 'config help' for more info\n"
           "uptime - get the uptime\n"
           "status - get the status of the system\n"
           "clearAlarm - clear the alarm\n"
           "resetStatus - reset reboot / blackout counters and clear alarm\n"
           "saveStatus - save status to persistent storage (SD card)\n";
}

String CommandLineInterface::ConfigCLI::executeCommand(String commandLine) {
    CommandAndArguments configCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    serialLogger.debug("CommandLineInterface::Config::executeCommand:'" + configCommand.command + "', args: '" +
                       configCommand.arguments + "'");

    if (configCommand.command.equals("") || configCommand.command.equals(CLI_COMMAND_CONFIG_GET)) {
        return getConfig(configCommand.arguments);
    } else if (configCommand.command.equals(CLI_COMMAND_CONFIG_SET)) {
        return setConfig(configCommand.arguments);
    } else if (configCommand.command.equals(CLI_COMMAND_CONFIG_SAVE)) {
        return saveConfig();
    } else if (configCommand.command.equals("help") || configCommand.command.equals("-h") || configCommand.command.equals("--help")) {
        return getHelp();
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
