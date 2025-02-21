#include "LogRotate.h"

void LogRotate::rotate() {
    if (shouldRotate()) {
        renameFiles();
        Logger::info("Rotated log files");
    }
}

void LogRotate::renameFiles() {
    for (unsigned long i = configuration.getLogRotateNumberOfFilesKept(); i > 0; i--) {
        String source = getFilePath(i - 1);
        String target = getFilePath(i);

        if (persistentStorage.exists(source)) {
            persistentStorage.rename(source, target);
        }
    }
}

bool LogRotate::shouldRotate() {
    return configuration.getLogRotateNumberOfFilesKept() > 0
           && persistentStorage.getSize(getFilePath()) >= configuration.getLogRotateMinimumFileSize();
}

String LogRotate::getFilePath(unsigned int sequence) {
    return configuration.getLogDirectory() + "/" + configuration.getLogFileName()
           + (sequence == 0 ? "" : "." + String(sequence));
}