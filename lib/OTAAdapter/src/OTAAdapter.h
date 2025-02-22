#ifndef BLACKOUT_DETECT_O_MATIC_OTA_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_OTA_ADAPTER_H

#include <WiFi.h>
#include <ArduinoOTA.h>

#include "Logger.h"

class OTAAdapter {
public:
    static void init();

    static void loop();

    static const String logTag;
};


#endif //BLACKOUT_DETECT_O_MATIC_OTA_ADAPTER_H
