#include "MemoryInfo.h"

String MemoryInfo::getFormattedMemoryInfo() {
    return "Total heap: " + String(ESP.getHeapSize()) + "\n"
           "Free heap: " + String(ESP.getFreeHeap()) + "\n"
           "Total PSRAM: " + String(ESP.getPsramSize()) + "\n"
           "Free PSRAM: " + String(ESP.getFreePsram());
}

JsonDocument MemoryInfo::toJsonDocument() {
    JsonDocument doc;

    doc[MEMORY_INFO_FIELD_HEAP][MEMORY_INFO_FIELD_HEAP_TOTAL] = ESP.getHeapSize();
    doc[MEMORY_INFO_FIELD_HEAP][MEMORY_INFO_FIELD_HEAP_FREE] = ESP.getFreeHeap();
    doc[MEMORY_INFO_FIELD_PSRAM][MEMORY_INFO_FIELD_PSRAM_TOTAL] = ESP.getPsramSize();
    doc[MEMORY_INFO_FIELD_PSRAM][MEMORY_INFO_FIELD_PSRAM_FREE] = ESP.getFreePsram();

    return doc;
}