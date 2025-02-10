#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "performance-unnecessary-value-param"

#include <Arduino.h>

#include "SerialLogger.h"
#include "UnixTimeWithMilliSeconds.h"

SerialLogger::SerialLogger() {
    Serial.begin(SERIAL_LOGGER_BAUD_RATE);
}

String SerialLogger::getLogHeader(String level) {
    UnixTimeWithMilliSeconds time = rtcAdapter.getTime();
    return time.getFormattedTime() + "." + time.getMillisAsZeroPaddedString() + " [" + level + "] ";
}

void SerialLogger::debug(String message) {
    if (logLevel >= DEBUG) {
        log("DEBUG", message);
    }
}

void SerialLogger::info(String message) {
    if (logLevel >= INFO) {
        log("INFO", message);
    }
}

void SerialLogger::error(String message) {
    if (logLevel >= ERROR) {
        log("ERROR", message);
    }
}

void SerialLogger::log(String level, String message) {
    Serial.println(getLogHeader(level) + message);
}

void SerialLogger::setLogLevel(String level) {
    logLevel = stringToLogLevel(level);
}

void SerialLogger::setLogLevel(SerialLogger::LogLevel level) {
    logLevel = level;
}

SerialLogger::LogLevel SerialLogger::stringToLogLevel(String level) {
    level.toUpperCase();
    if (level == "NONE") {
        return SerialLogger::NONE;
    } else if (level == "ERROR") {
        return SerialLogger::ERROR;
    } else if (level == "INFO") {
        return SerialLogger::INFO;
    } else {
        return SerialLogger::DEBUG;
    }
}

String SerialLogger::logLevelToString(SerialLogger::LogLevel level) {
    switch(level) {
        case SerialLogger::NONE:
            return "NONE";
        case SerialLogger::ERROR:
            return "ERROR";
        case SerialLogger::INFO:
            return "INFO";
        default:
            return "DEBUG";
    }
}

SerialLogger serialLogger = SerialLogger();

#pragma clang diagnostic pop