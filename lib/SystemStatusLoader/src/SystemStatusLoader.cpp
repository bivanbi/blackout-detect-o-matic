#include "SystemStatusLoader.h"

bool SystemStatusLoader::load() {
    String path = configuration.getSystemStatusFilePath();
    JsonDocument doc = getSystemStatusFile(path);

    if (doc.isNull()) {
        return false;
    }

    systemStatus = SystemStatus(doc.as<JsonObject>());
    Logger::info("SystemStatusLoader::load: loaded status from file '" + path + "'");
    return true;
}

bool SystemStatusLoader::save() {
    String path = configuration.getSystemStatusFilePath();
    JsonDocument doc = systemStatus.toJsonDocument();
    JsonDocument docOnDisk = getSystemStatusFile(path);

    if (doc.as<String>() == docOnDisk.as<String>()) {
        Logger::info("SystemStatusLoader::save: status has not changed, not saving to file '" + path + "' to reduce SD card wear");
        return true;
    }

    if (!persistentStorage.writeFile(path, doc.as<String>())) {
        Logger::error("SystemStatusLoader::save: failed to save status to file '" + path + "'");
        return false;
    }

    Logger::info("SystemStatusLoader::save: saved status to file '" + path + "'");
    return true;
}

JsonDocument SystemStatusLoader::getSystemStatusFile(const String &path) {
    JsonDocument doc;

    if (!persistentStorage.exists(path)) {
        Logger::error("SystemStatusLoader::getSystemStatusFile: file '" + path + "': does not exist");
        return doc;
    }

    String content = persistentStorage.readFile(path);
    if (content.length() == 0) {
        Logger::error("SystemStatusLoader::getSystemStatusFile: file '" + path + "': has zero length");
        return doc;
    }

    DeserializationError error = deserializeJson(doc, content);
    if (error) {
        Logger::error("SystemStatusLoader::getSystemStatusFile: Failed to deserialize file '" + path + "': " +
                           String(error.c_str()));
    }
    return doc;
}
