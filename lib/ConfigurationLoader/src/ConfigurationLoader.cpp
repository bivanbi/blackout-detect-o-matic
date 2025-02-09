#include "ConfigurationLoader.h"

bool ConfigurationLoader::load() {
    JsonDocument doc = getConfigurationFile(CONFIGURATION_FILE_PATH);

    if (doc.isNull()) {
        return false;
    }

    configuration = Configuration(doc.as<JsonObject>());
    serialLogger.info("ConfigurationLoader::load: loaded configuration from file '" CONFIGURATION_FILE_PATH "'");
    return true;
}

bool ConfigurationLoader::save() {
    JsonDocument doc = configuration.toJsonDocument();
    JsonDocument docOnDisk = getConfigurationFile(CONFIGURATION_FILE_PATH);

    if (doc.as<String>() == docOnDisk.as<String>()) {
        serialLogger.info("ConfigurationLoader::save: configuration has not changed, not saving to file '" CONFIGURATION_FILE_PATH "' to reduce SD card wear");
        return true;
    }

    if (!persistentStorage.writeFile(CONFIGURATION_FILE_PATH, doc.as<String>())) {
        serialLogger.error("ConfigurationLoader::save: failed to configuration status to file '" CONFIGURATION_FILE_PATH "'");
        return false;
    }

    serialLogger.info("ConfigurationLoader::save: saved configuration to file '" CONFIGURATION_FILE_PATH "'");
    return true;
}

JsonDocument ConfigurationLoader::getConfigurationFile(const String &path) {
    JsonDocument doc;

    if (!persistentStorage.exists(path)) {
        serialLogger.error("ConfigurationLoader::getConfigurationFile: file '" + path + "': does not exist");
        return doc;
    }

    String content = persistentStorage.readFile(path);
    if (content.length() == 0) {
        serialLogger.error("ConfigurationLoader::getConfigurationFile: file '" + path + "': has zero length");
        return doc;
    }

    DeserializationError error = deserializeJson(doc, content);
    if (error) {
        serialLogger.error("ConfigurationLoader::getConfigurationFile: Failed to deserialize file '" + path + "': " +
                           String(error.c_str()));
    }
    return doc;
}
