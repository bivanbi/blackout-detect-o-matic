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

bool PersistentStorage::exists(const String& path) {
    return SD_MMC.exists(path);
}

bool PersistentStorage::createDirectory(const String& path) {
    return SD_MMC.mkdir(path);
}

bool PersistentStorage::removeDirectory(const String& path) {
    return SD_MMC.rmdir(path);
}

bool PersistentStorage::isDirectory(const String& path) {
    return SD_MMC.open(path).isDirectory();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
String PersistentStorage::listDirectory(const String& dirname, int maxDepth) {
    File root = SD_MMC.open(dirname);
    if (!root) {
        return {"Failed to open directory " + dirname + "\n"};
    }

    if (!root.isDirectory()) {
        return "";
    }

    String result = "";

    File file = root.openNextFile();
    while (file) {
        //String pathPrepend = dirname.equals("/") ? "" : dirname;
        UnixTimeWithMilliSeconds lastModified = getLastModificationDate(file.path());
        String size = "  SIZE: " + String(file.size());
        result += formatDirFileLine(file) + "\n";

        if (file.isDirectory()) {
            if (maxDepth > 0) {
                result += listDirectory(file.path(), maxDepth - 1);
            }
        }
        file = root.openNextFile();
    }

    return result;
}

String PersistentStorage::formatDirFileLine(fs::File file) {
    const char *type = file.isDirectory() ? "d" : "f";

    char buffer[256];

    snprintf(buffer, sizeof(buffer), "%s %10u %s %s",
             type, file.size(), getLastModificationDate(file).getFormattedTime().c_str(), file.path());
    return {buffer};
}

bool PersistentStorage::recursiveRemovePath(const String& path, int maxDepth) {
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

String PersistentStorage::readFile(const String& path) {
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

bool PersistentStorage::writeFile(const String& path, const String& content) {
    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        return false;
    }

    size_t bytesWritten = file.print(content);
    file.close();
    return bytesWritten == content.length();
}

bool PersistentStorage::appendFile(const String& path, const String& content) {
    File file = SD_MMC.open(path, FILE_APPEND);
    if (!file) {
        return false;
    }

    size_t bytesWritten = file.print(content);
    file.close();
    return bytesWritten == content.length();
}

bool PersistentStorage::removeFile(const String& path) {
    return SD_MMC.remove(path);
}

bool PersistentStorage::rename(const String &pathFrom, const String &pathTo) {
    return SD_MMC.rename(pathFrom, pathTo);
}

uint64_t PersistentStorage::getSize(const String& path) {
    File file = SD_MMC.open(path);
    if (!file) {
        return 0;
    }

    uint64_t size = file.size();
    file.close();
    return size;
}

UnixTimeWithMilliSeconds PersistentStorage::getLastModificationDate(const String& path) {
    File file = SD_MMC.open(path);
    UnixTimeWithMilliSeconds result = getLastModificationDate(file);
    file.close();
    return result;
}

UnixTimeWithMilliSeconds PersistentStorage::getLastModificationDate(File file) {
    if (!file) {
        return {};
    }

    time_t lastModificationTime = file.getLastWrite();
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