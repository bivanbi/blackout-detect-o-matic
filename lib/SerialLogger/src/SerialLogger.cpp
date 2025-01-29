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
    log("DEBUG", message);
}

void SerialLogger::info(String message) {
    log("INFO", message);
}

void SerialLogger::error(String message) {
    log("ERROR", message);
}

void SerialLogger::log(String level, String message) {
    Serial.println(getLogHeader(level) + message);
}

SerialLogger serialLogger = SerialLogger();

#pragma clang diagnostic pop