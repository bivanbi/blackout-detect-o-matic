#ifndef BLACKOUT_DETECT_O_MATIC_PERSISTENT_STORAGE_H
#define BLACKOUT_DETECT_O_MATIC_PERSISTENT_STORAGE_H

#include <Arduino.h>
#include <SD_MMC.h>

#include "UnixTimeWithMilliSeconds.h"

#define SD_MMC_CMD 15 // Freenove ESP32 Wrover pinout. Adjust it to the actual board
#define SD_MMC_CLK 14
#define SD_MMC_D0  2

#define SD_MMC_MOUNT_POINT /sdcard

class PersistentStorage {
public:
    PersistentStorage() = default;

    bool mount();

    bool isMounted();

    bool exists(String path);

    bool createDirectory(String path);

    bool removeDirectory(String path);

    bool recursiveRemovePath(String path, int maxDepth = 99);

    void unMount();

    bool writeFile(String path, String content);

    bool appendFile(String path, String content);

    String readFile(String path);

    bool removeFile(String path);

    UnixTimeWithMilliSeconds getLastModificationDate(String path);

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

#endif //BLACKOUT_DETECT_O_MATIC_PERSISTENT_STORAGE_H
