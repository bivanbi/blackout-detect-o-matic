#ifndef BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
#define BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H

#include <WString.h>

#include "SystemStatus.h"
#include "RTCAdapter.h"
#include "SerialLogger.h"
#include "SystemStatusLoader.h"

#define CLI_COMMAND_CONFIG "config"
#define CLI_COMMAND_CONFIG_GET "get"

#define CLI_RESPONSE_UNKNOWN_COMMAND "unknown command"

class CommandLineInterface {
    class ConfigCLI {
    public:
        ConfigCLI() = default;

        String executeCommand(String command);

        String getConfig();

        String getHelp();
    };

public:
    struct CommandAndArguments {
        String command;
        String arguments;
    };

    CommandLineInterface() = default;

    CommandAndArguments splitCommandAndArguments(String command);

    String executeCommand(String command);

    String getStatus();

    String getHelp();

private:
    ConfigCLI config;
};

extern CommandLineInterface commandLineInterface;

#endif //BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
