#ifndef BLACKOUT_DETECT_O_MATIC_LOGGER_H
#define BLACKOUT_DETECT_O_MATIC_LOGGER_H

#include <Arduino.h>
#include <iostream>
#include <initializer_list>
#include <utility>

#include "RTCAdapter.h"
#include "LogSeverity.h"
#include "LogHelper.h"
#include "Logger.h"
#include "PersistentLogger.h"
#include "SerialLogger.h"

class Logger {
public:
    enum Channel {
        ALL = 0,
        SERIAL_PORT = 1,
        PERSISTENT_STORAGE = 2
    };

    static void begin();

    static void error(const String& message, std::initializer_list<Logger::Channel> channels = {Logger::Channel::ALL});

    static void info(const String& message, std::initializer_list<Logger::Channel> channels = {Logger::Channel::ALL});

    static void debug(const String& message, std::initializer_list<Logger::Channel> channels = {Logger::Channel::ALL});

    static void log(LogSeverity::Severity severity, const String& message, std::initializer_list<Logger::Channel> channels = {Logger::Channel::ALL});

    static void setLogLevel(String level, std::initializer_list<Logger::Channel> channels = {Logger::Channel::ALL});

    static void setLogLevel(LogSeverity::Severity level, std::initializer_list<Logger::Channel> channels = {Logger::Channel::ALL});
};

#endif //BLACKOUT_DETECT_O_MATIC_LOGGER_H
