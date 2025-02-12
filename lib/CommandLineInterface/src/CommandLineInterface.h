#ifndef BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
#define BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H

#include <WString.h>

#include "SystemStatus.h"
#include "RTCAdapter.h"
#include "SerialLogger.h"
#include "SystemStatusLoader.h"
#include "ConfigurationLoader.h"
#include "PeriodicTaskScheduler.h"
#include "UptimeAdapter.h"
#include "MemoryInfo.h"

#define CLI_COMMAND_ALARM "alarm"
#define CLI_COMMAND_ALARM_CLEAR "clear"

#define CLI_COMMAND_CONFIG "config"
#define CLI_COMMAND_CONFIG_GET "get"
#define CLI_COMMAND_CONFIG_SET "set"
#define CLI_COMMAND_CONFIG_SAVE "save"

#define CLI_COMMAND_DATE "date"

#define CLI_COMMAND_HELP "help"
#define CLI_COMMAND_MEMINFO "meminfo"

#define CLI_COMMAND_REBOOT "reboot"
#define CLI_DEFAULT_REBOOT_DELAY 3 // seconds

#define CLI_COMMAND_SDCARD "sdcard"
#define CLI_COMMAND_SDCARD_CAT "cat"
#define CLI_COMMAND_SDCARD_LIST "list"
#define CLI_COMMAND_SDCARD_REMOVE "remove"
#define CLI_COMMAND_SDCARD_USAGE "usage"

#define CLI_COMMAND_STATUS "status"
#define CLI_COMMAND_STATUS_GET "get"
#define CLI_COMMAND_STATUS_RESET "reset"
#define CLI_COMMAND_STATUS_SAVE "save"

#define CLI_COMMAND_UPTIME "uptime"

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

    String date();

    String help();

    String uptime();

    class AlarmCLI {
    public:
        AlarmCLI() = default;

        String executeCommand(String command);

        String clear();

        String help();
    };

    class ConfigCLI {
    public:
        ConfigCLI() = default;

        String executeCommand(String command);

        String get();

        String get(String key);

        String set(String keyAndValue);

        String save();

        String help();
    };

    class SDCardCLI {
    public:
        SDCardCLI() = default;

        String executeCommand(String command);

        String cat(String filename);

        String list(String directory);

        String remove(String filename);

        String usage();

        String help();
    };

    class StatusCLI {
    public:
        StatusCLI() = default;

        String executeCommand(String command);

        String get();

        String reset();

        String save();

        String help();
    };

private:
    AlarmCLI alarm;
    ConfigCLI config;
    SDCardCLI sdcard;
    StatusCLI status;

    /**
     * Schedule reboot
     * @param delay in seconds
     */
    String scheduleReboot(CommandAndArguments cmd);
};

extern CommandLineInterface commandLineInterface;

#endif //BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
