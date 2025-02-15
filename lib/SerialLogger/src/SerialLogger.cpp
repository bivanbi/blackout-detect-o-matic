#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "performance-unnecessary-value-param"

#include "SerialLogger.h"

void SerialLogger::begin() {
    logLevel = LogSeverity::Severity::INFO;
    Serial.begin(SERIAL_LOGGER_BAUD_RATE);
}

void SerialLogger::log(LogSeverity::Severity severity, String message) {
    if (severity <= logLevel) {
        Serial.println(LogHelper::getLogHeader(severity) + message);
    }
}

void SerialLogger::setLogLevel(LogSeverity::Severity level) {
    logLevel = level;
}

LogSeverity::Severity SerialLogger::logLevel = LogSeverity::Severity::INFO;

#pragma clang diagnostic pop