#ifndef BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
#define BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H

#include <WString.h>

#include "SystemStatus.h"
#include "RTCAdapter.h"
#include "SerialLogger.h"
#include "SystemStatusLoader.h"
#include "ConfigurationLoader.h"

#define CLI_COMMAND_CONFIG "config"
#define CLI_COMMAND_CONFIG_GET "get"
#define CLI_COMMAND_CONFIG_SET "set"
#define CLI_COMMAND_CONFIG_SAVE "save"

#define CLI_RESPONSE_UNKNOWN_COMMAND "unknown command"

class CommandLineInterface {
public:
    struct CommandAndArguments {
        String command;
        String arguments;
    };

    struct KeyValue {
        String key;
        String value;
    };

    CommandLineInterface() = default;

    CommandAndArguments splitCommandAndArguments(String command);

    KeyValue splitKeyValuePair(String keyAndValue);

    String executeCommand(String command);

    String getStatus();

    String getUptime();

    String getHelp();

    class ConfigCLI {
    public:
        ConfigCLI() = default;

        String executeCommand(String command);

        String getConfig();

        String getConfig(String key);

        String setConfig(String keyAndValue);

        String saveConfig();

        String getHelp();
    };


private:
    ConfigCLI config;
};

extern CommandLineInterface commandLineInterface;

#endif //BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
