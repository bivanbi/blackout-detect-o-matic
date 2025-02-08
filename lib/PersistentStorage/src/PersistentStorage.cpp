#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#include "PersistentStorage.h"

bool PersistentStorage::mount() {
    return SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0)
           && SD_MMC.begin("/sdcard", true, false, SDMMC_FREQ_DEFAULT, 5);
}

void PersistentStorage::unMount() {
    SD_MMC.end();
}

bool PersistentStorage::isMounted() {
    return SD_MMC.exists("/");
}

bool PersistentStorage::exists(String path) {
    return SD_MMC.exists(path);
}

bool PersistentStorage::createDirectory(String path) {
    return SD_MMC.mkdir(path);
}

bool PersistentStorage::removeDirectory(String path) {
    return SD_MMC.rmdir(path);
}

bool PersistentStorage::recursiveRemovePath(String path, int maxDepth) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
    if (maxDepth <= 0) { // prevent endless loops
        return false;
    }

    File root = SD_MMC.open(path);
    if (!root) { // failed to open
        return false;
    }

    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            recursiveRemovePath(file.path(), maxDepth--);
            file = root.openNextFile();
        }
        return SD_MMC.rmdir(path);
    } else {
        return SD_MMC.remove(path);
    }
}
#pragma clang diagnostic pop

String PersistentStorage::readFile(String path) {
    File file = SD_MMC.open(path);
    if (!file) {
        return "";
    }

    String content = "";
    while (file.available()) {
        int nextChar = file.read();
        content += char(nextChar);
    }
    file.close();
    return content;
}

bool PersistentStorage::writeFile(String path, String content) {
    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        return false;
    }

    size_t bytesWritten = file.print(content);
    file.close();
    return bytesWritten == content.length();
}

bool PersistentStorage::appendFile(String path, String content) {
    File file = SD_MMC.open(path, FILE_APPEND);
    if (!file) {
        return false;
    }

    size_t bytesWritten = file.print(content);
    file.close();
    return bytesWritten == content.length();
}

bool PersistentStorage::removeFile(String path) {
    return SD_MMC.remove(path);
}

UnixTimeWithMilliSeconds PersistentStorage::getLastModificationDate(String path) {
    File file = SD_MMC.open(path);
    if (!file) {
        return UnixTimeWithMilliSeconds();
    }

    time_t lastModificationTime = file.getLastWrite();
    file.close();
    return UnixTimeWithMilliSeconds(lastModificationTime);
}

uint64_t PersistentStorage::getTotalSpace() {
    return SD_MMC.totalBytes();
}

uint64_t PersistentStorage::getUsedSpace() {
    return SD_MMC.usedBytes();
}

uint64_t PersistentStorage::getFreeSpace() {
    return SD_MMC.totalBytes() - SD_MMC.usedBytes();
}

PersistentStorage persistentStorage;
#pragma clang diagnostic pop