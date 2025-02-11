#ifndef BLACKOUT_DETECT_O_MATIC_PERSISTENT_STORAGE_H
#define BLACKOUT_DETECT_O_MATIC_PERSISTENT_STORAGE_H

#include <Arduino.h>
#include <SD_MMC.h>

#include "UnixTimeWithMilliSeconds.h"

#define SD_MMC_CMD 15 // Freenove ESP32 Wrover pinout. Adjust it to the actual board
#define SD_MMC_CLK 14
#define SD_MMC_D0  2

class PersistentStorage {
public:
    PersistentStorage() = default;

    bool mount();

    bool isMounted();

    bool exists(const String& path);

    bool createDirectory(const String& path);

    bool removeDirectory(const String& path);

    bool recursiveRemovePath(const String& path, int maxDepth = 99);

    void unMount();

    bool writeFile(const String& path, const String& content);

    bool appendFile(const String& path, const String& content);

    String readFile(const String& path);

    bool removeFile(const String& path);

    UnixTimeWithMilliSeconds getLastModificationDate(const String& path);

    /**
     * Get total space on the storage media.
     * @return free space in bytes
     */
    uint64_t getTotalSpace();

    /**
     * Get used space on the storage media.
     * @return used space in bytes
     */
    uint64_t getUsedSpace();

    /**
     * Get free space on the storage media.
     * @return free space in bytes
     */
    uint64_t getFreeSpace();
};

extern PersistentStorage persistentStorage;

#endif //BLACKOUT_DETECT_O_MATIC_PERSISTENT_STORAGE_H
