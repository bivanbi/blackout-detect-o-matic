#ifndef BLACKOUT_DETECT_O_MATIC_MEMORY_INFO_H
#define BLACKOUT_DETECT_O_MATIC_MEMORY_INFO_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define MEMORY_INFO_FIELD_HEAP "heap"
#define MEMORY_INFO_FIELD_HEAP_TOTAL "total"
#define MEMORY_INFO_FIELD_HEAP_FREE "free"
#define MEMORY_INFO_FIELD_PSRAM "psram"
#define MEMORY_INFO_FIELD_PSRAM_TOTAL "total"
#define MEMORY_INFO_FIELD_PSRAM_FREE "free"

class MemoryInfo {
public:
    static String getFormattedMemoryInfo();

    static JsonDocument toJsonDocument();
};

#endif //BLACKOUT_DETECT_O_MATIC_MEMORY_INFO_H
