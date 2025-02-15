#include "PersistentLogger.h"

void PersistentLogger::begin() {
    logFilePath = configuration.getLogDirectory() + "/" + configuration.getLogFileName();
    logLevel = LogSeverity::Severity::INFO;
}

void PersistentLogger::log(LogSeverity::Severity severity, const String& message) {
    if (severity <= logLevel) {
        persistentStorage.appendFile(logFilePath, LogHelper::getLogHeader(severity) + message + "\n");
    }
}

void PersistentLogger::setLogLevel(LogSeverity::Severity level) {
    logLevel = level;
}

String PersistentLogger::logFilePath;
LogSeverity::Severity PersistentLogger::logLevel;