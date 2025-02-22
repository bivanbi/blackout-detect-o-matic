#include "Logger.h"

void Logger::begin() {
    SerialLogger::begin();
    PersistentLogger::begin();
}

void Logger::error(const String& message, std::initializer_list<Logger::Channel> channels) {
    log(LogSeverity::Severity::ERROR, message, channels);
}

void Logger::info(const String& message, std::initializer_list<Logger::Channel> channels) {
    log(LogSeverity::INFO, message, channels);
}

void Logger::debug(const String& message, std::initializer_list<Logger::Channel> channels) {
    log(LogSeverity::DEBUG, message, channels);
}

void Logger::log(LogSeverity::Severity severity, const String& message, std::initializer_list<Logger::Channel> channels) {
    for (auto &channel : channels) {
        switch (channel) {
            case Logger::Channel::PERSISTENT_STORAGE:
                PersistentLogger::log(severity, message);
                break;
            case Logger::Channel::SERIAL_PORT:
                SerialLogger::log(severity, message);
                break;
            default:
                SerialLogger::log(severity, message);
                PersistentLogger::log(severity, message);
        }
    }
}

void Logger::setLogLevel(LogSeverity::Severity level, std::initializer_list<Logger::Channel> channels) {
    for (auto &channel : channels) {
        switch (channel) {
            case Logger::Channel::PERSISTENT_STORAGE:
                PersistentLogger::setLogLevel(level);
                break;
            case Logger::Channel::SERIAL_PORT:
                SerialLogger::setLogLevel(level);
                break;
            default:
                SerialLogger::setLogLevel(level);
                PersistentLogger::setLogLevel(level);
        }
    }
}

void Logger::setLogLevel(String level, std::initializer_list<Logger::Channel> channels) {
    setLogLevel(LogSeverity::stringToSeverity(std::move(level)), channels);
}

