#ifndef BLACKOUT_DETECT_O_MATIC_CONFIGURATION_LOADER_H
#define BLACKOUT_DETECT_O_MATIC_CONFIGURATION_LOADER_H

#include <Arduino.h>

#include "Configuration.h"
#include "Logger.h"
#include "PersistentStorage.h"

class ConfigurationLoader {
public:
    static bool load();

    static bool save();

private:
    static JsonDocument getConfigurationFile(const String &path);
    static const String logTag;
};

#endif //BLACKOUT_DETECT_O_MATIC_CONFIGURATION_LOADER_H
