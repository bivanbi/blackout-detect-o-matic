#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "CommandLineInterface.h"

const String CommandLineInterface::logTag = "CommandLineInterface: ";

String CommandLineInterface::executeCommand(String commandLine) {
    commandLine.trim();
    CommandAndArguments cmd = splitCommandAndArguments(commandLine);

    if (cmd.command.equals(CLI_COMMAND_ALARM)) {
        return alarm.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_CONFIG)) {
        return config.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_DATE)) {
        return date();
    } else if (cmd.command.equals(CLI_COMMAND_HELP)) {
        return help();
    } else if (cmd.command.equals(CLI_COMMAND_MEMINFO)) {
        return MemoryInfo::getFormattedMemoryInfo();
    } else if (cmd.command.equals(CLI_COMMAND_REBOOT)) {
        return scheduleReboot(cmd);
    } else if (cmd.command.equals(CLI_COMMAND_SDCARD)) {
        return sdcard.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_STATUS)) {
        return status.executeCommand(cmd.arguments);
    } else if (cmd.command.equals(CLI_COMMAND_UPTIME)) {
        return uptime();
    } else if (cmd.command.isEmpty()) {
        return "";
    } else {
        return CLI_RESPONSE_UNKNOWN_COMMAND;
    }
}

String CommandLineInterface::date() {
    return "Clock source: " + rtcAdapter.clockSourceToString(rtcAdapter.getClockSource())
           + ", time: " + rtcAdapter.getTime().getFormattedTime() + "\n";
}

String CommandLineInterface::help() {
    return CLI_COMMAND_ALARM " - alarm commands, issue 'alarm help' for more info\n"
           CLI_COMMAND_CONFIG " - configuration commands, issue 'config help' for more info\n"
           CLI_COMMAND_DATE " - get the current system date and time\n"
           CLI_COMMAND_MEMINFO " - get memory usage info\n"
           CLI_COMMAND_REBOOT " [delay] - reboot the system with optional delay in seconds, default: " + String(CLI_DEFAULT_REBOOT_DELAY) + " seconds\n"
           CLI_COMMAND_SDCARD " - SD Card commands, issue 'sdcard help' for more info\n"
           CLI_COMMAND_STATUS " - status commands, issue 'status help' for more info\n"
           CLI_COMMAND_UPTIME " - get the uptime\n";
}

String CommandLineInterface::scheduleReboot(CommandLineInterface::CommandAndArguments cmd) {
        if (cmd.arguments.isEmpty()) {
            cmd.arguments = String(CLI_DEFAULT_REBOOT_DELAY);
        }

        PeriodicTaskScheduler::scheduleReboot(cmd.arguments.toInt());
        return "Rebooting in " + String(CLI_DEFAULT_REBOOT_DELAY) + " seconds\n";
}

String CommandLineInterface::uptime() {
    return rtcAdapter.getTime().getFormattedTime() + " up "
           + UptimeAdapter::getFormattedUptime() + "\n";
}

String CommandLineInterface::AlarmCLI::executeCommand(String commandLine) {
    CommandAndArguments alarmCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    Logger::debug(logTag + "Alarm::executeCommand:'" + alarmCommand.command + "', args: '" +
                       alarmCommand.arguments + "'");

    if (alarmCommand.command.equals(CLI_COMMAND_ALARM_CLEAR)) {
        return clear();
    } else if (alarmCommand.command.equals(CLI_COMMAND_HELP)) {
        return help();
    } else if (alarmCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::AlarmCLI::clear() {
    systemStatus.clearAlarm(rtcAdapter.getTime());
    return "Alarm cleared\n";
}

String CommandLineInterface::AlarmCLI::help() {
    return "alarm clear - clear the alarm\n";
}

String CommandLineInterface::ConfigCLI::executeCommand(String commandLine) {
    CommandAndArguments configCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    Logger::debug(logTag + "Config::executeCommand:'" + configCommand.command + "', args: '" +
                       configCommand.arguments + "'");

    if (configCommand.command.equals(CLI_COMMAND_CONFIG_GET)) {
        return get(configCommand.arguments);
    } else if (configCommand.command.equals(CLI_COMMAND_CONFIG_SET)) {
        return set(configCommand.arguments);
    } else if (configCommand.command.equals(CLI_COMMAND_CONFIG_SAVE)) {
        return save();
    } else if (configCommand.command.equals(CLI_COMMAND_HELP)) {
        return help();
    } else if (configCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::ConfigCLI::get(String key) {
    if (key.isEmpty()) {
        return get();
    }

    Configuration::GetResult result = configuration.get(key);
    if (result.code == Configuration::ResultCode::OK) {
        return result.value + "\n";
    }

    Logger::error(logTag + "ConfigCLI::get: key: '" + key + "', error message: " + result.value);
    return result.value + "\n";
}

String CommandLineInterface::ConfigCLI::get() {
    String result;
    serializeJsonPretty(configuration.toJsonDocument(), result);
    return result + "\n";
}

String CommandLineInterface::ConfigCLI::set(String keyAndValue) {
    KeyValue keyValuePair = commandLineInterface.splitKeyValuePair(keyAndValue);
    if (keyValuePair.key.isEmpty()) {
        return "ERROR: expected key=value pair\n";
    }

    Configuration::SetResult result = configuration.set(keyValuePair.key, keyValuePair.value);
    if (result.code == Configuration::ResultCode::OK) {
        return "Configuration updated: " + keyAndValue + "\n";
    }

    Logger::error(logTag + "ConfigCLI::set: "
                       "key: '" + keyValuePair.key + "', value: '" + keyValuePair.value +
                       "', error message: " + result.message);
    return "ERROR: " + result.message + "\n";
}

String CommandLineInterface::ConfigCLI::save() {
    if (ConfigurationLoader::save()) {
        return "Configuration saved\n";
    }

    return "Failed to save configuration\n";
}

String CommandLineInterface::ConfigCLI::help() {
    return CLI_COMMAND_CONFIG " " CLI_COMMAND_CONFIG_GET " - get the current configuration\n"
           CLI_COMMAND_CONFIG " " CLI_COMMAND_CONFIG_GET " <key> - get the value of a specific key\n"
           CLI_COMMAND_CONFIG " " CLI_COMMAND_CONFIG_SET " <key>=<value> - set the value of a specific key\n"
           CLI_COMMAND_CONFIG " " CLI_COMMAND_CONFIG_SAVE " - save the configuration to persistent storage (SD card)\n\n"
           "Note: Save and reboot to apply the updated configuration\n";
}

String CommandLineInterface::SDCardCLI::executeCommand(String commandLine) {
    CommandAndArguments sdCardCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    Logger::debug(logTag + "SDCard::executeCommand:'" + sdCardCommand.command + "', args: '" +
                       sdCardCommand.arguments + "'");

    if (sdCardCommand.command.equals(CLI_COMMAND_SDCARD_CAT)) {
        return cat(sdCardCommand.arguments);
    } else if (sdCardCommand.command.equals(CLI_COMMAND_SDCARD_LIST)) {
        return list(sdCardCommand.arguments);
    } else if (sdCardCommand.command.equals(CLI_COMMAND_SDCARD_REMOVE)) {
        return remove(sdCardCommand.arguments);
    } else if (sdCardCommand.command.equals(CLI_COMMAND_SDCARD_USAGE)) {
        return usage();
    } else if (sdCardCommand.command.equals(CLI_COMMAND_HELP)) {
        return help();
    } else if (sdCardCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::SDCardCLI::cat(String filename) {
    if (filename.isEmpty()) {
        return "ERROR: missing file name\n";
    }

    if (!persistentStorage.exists(filename)) {
        return "ERROR: file not found\n";
    }

    if (persistentStorage.isDirectory(filename)) {
        return "ERROR: not a file\n";
    }

    return persistentStorage.readFile(filename) + "\n";
}

String CommandLineInterface::SDCardCLI::list(String directory) {
    if (directory.isEmpty()) {
        directory = "/";
    }

    return persistentStorage.listDirectory(directory);
}

String CommandLineInterface::SDCardCLI::remove(String fileName) {
    if (fileName.isEmpty()) {
        return "ERROR: missing file name\n";
    }

    if (!persistentStorage.exists(fileName)) {
        return "ERROR: file not found\n";
    }

    if (persistentStorage.isDirectory(fileName)) {
        return "ERROR: not a file\n";
    }

    if (persistentStorage.removeFile(fileName)) {
        return "File removed\n";
    }

    return "Failed to remove file\n";
}

String CommandLineInterface::SDCardCLI::usage() {
    return "Total space: " + String(persistentStorage.getTotalSpace()) + " bytes\n"
           "Used space: " + String(persistentStorage.getUsedSpace()) + " bytes\n"
           "Free space: " + String(persistentStorage.getFreeSpace()) + " bytes\n";
}

String CommandLineInterface::SDCardCLI::help() {
    return CLI_COMMAND_SDCARD " " CLI_COMMAND_SDCARD_CAT " <filename> - read the content of a file\n"
           CLI_COMMAND_SDCARD " " CLI_COMMAND_SDCARD_LIST " [directory] - list the files in the directory, default directory: /\n"
           CLI_COMMAND_SDCARD " " CLI_COMMAND_SDCARD_REMOVE " <filename> - remove file from SD card\n"
           CLI_COMMAND_SDCARD " " CLI_COMMAND_SDCARD_USAGE " - get the SD card usage info\n";
}

String CommandLineInterface::StatusCLI::executeCommand(String commandLine) {
    CommandAndArguments statusCommand = commandLineInterface.splitCommandAndArguments(commandLine);
    Logger::debug(logTag + "Status::executeCommand:'" + statusCommand.command + "', args: '" +
                       statusCommand.arguments + "'");

    if (statusCommand.command.equals(CLI_COMMAND_STATUS_GET)) {
        return get();
    } else if (statusCommand.command.equals(CLI_COMMAND_STATUS_RESET)) {
        return reset();
    } else if (statusCommand.command.equals(CLI_COMMAND_STATUS_SAVE)) {
        return save();
    } else if (statusCommand.command.equals(CLI_COMMAND_HELP)) {
        return help();
    } else if (statusCommand.command.isEmpty()) {
        return "";
    }
    return CLI_RESPONSE_UNKNOWN_COMMAND;
}

String CommandLineInterface::StatusCLI::get() {
    String result;
    serializeJsonPretty(systemStatus.toHumanReadableJsonDocument(), result);
    return result;
}

String CommandLineInterface::StatusCLI::reset() {
    systemStatus.reset(rtcAdapter.getTime());
    return "Status reset\n";
}

String CommandLineInterface::StatusCLI::save() {
    if (SystemStatusLoader::save()) {
        return "Status saved\n";
    }

    return "Failed to save status\n";
}

String CommandLineInterface::StatusCLI::help() {
    return CLI_COMMAND_STATUS " " CLI_COMMAND_STATUS_GET " - get the current status\n"
           CLI_COMMAND_STATUS " " CLI_COMMAND_STATUS_RESET " - reset the status counters\n"
           CLI_COMMAND_STATUS " " CLI_COMMAND_STATUS_SAVE " - save the status to persistent storage (SD card)\n";
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
