#ifndef BLACKOUT_DETECT_O_MATIC_SYSTEM_STATUS_LOADER_H
#define BLACKOUT_DETECT_O_MATIC_SYSTEM_STATUS_LOADER_H

#include <Arduino.h>

#include "Configuration.h"
#include "Logger.h"
#include "SystemStatus.h"
#include "PersistentStorage.h"

class SystemStatusLoader {
public:
    static bool load();

    static bool save();

private:
    static JsonDocument getSystemStatusFile(const String &path);
};

#endif //BLACKOUT_DETECT_O_MATIC_SYSTEM_STATUS_LOADER_H
