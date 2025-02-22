#include "PersistentLogger.h"

void PersistentLogger::begin() {
    logLevel = LogSeverity::Severity::INFO;
}

void PersistentLogger::log(LogSeverity::Severity severity, const String& message) {
    if (severity <= logLevel) {
        persistentStorage.appendFile(getLogFilePath(), LogHelper::getLogHeader(severity) + message + "\n");
    }
}

void PersistentLogger::setLogLevel(LogSeverity::Severity level) {
    logLevel = level;
}

String PersistentLogger::getLogFilePath() {
    return configuration.getLogDirectory() + "/" + configuration.getLogFileName();
}

LogSeverity::Severity PersistentLogger::logLevel;