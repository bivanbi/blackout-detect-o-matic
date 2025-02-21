#ifndef BLACKOUT_DETECT_O_MATIC_LOGROTATE_H
#define BLACKOUT_DETECT_O_MATIC_LOGROTATE_H

#include <Arduino.h>

#include "Configuration.h"
#include "PersistentStorage.h"
#include "Logger.h"
#include "PersistentLogger.h"

class LogRotate {
public:
    static void rotate();

    static bool shouldRotate();

    /**
     * Get log file path for given sequence number. If sequence is 0, return current log file path.
     *
     * @param sequence
     * @return path to log file
     */
    static String getFilePath(unsigned int sequence = 0);

private:
    static void renameFiles();
};

#endif //BLACKOUT_DETECT_O_MATIC_LOGROTATE_H
