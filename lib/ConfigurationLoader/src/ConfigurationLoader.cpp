#include "ConfigurationLoader.h"

bool ConfigurationLoader::load() {
    JsonDocument doc = getConfigurationFile(CONFIGURATION_FILE_PATH);

    if (doc.isNull()) {
        return false;
    }

    configuration = Configuration(doc.as<JsonObject>());
    Logger::info("ConfigurationLoader::load: loaded configuration from file '" CONFIGURATION_FILE_PATH "'", {Logger::Channel::SERIAL_PORT});
    return true;
}

bool ConfigurationLoader::save() {
    JsonDocument doc = configuration.toJsonDocument();
    JsonDocument docOnDisk = getConfigurationFile(CONFIGURATION_FILE_PATH);

    if (doc.as<String>() == docOnDisk.as<String>()) {
        Logger::info("ConfigurationLoader::save: configuration has not changed, not saving to file '" CONFIGURATION_FILE_PATH "' to reduce SD card wear");
        return true;
    }

    if (!persistentStorage.writeFile(CONFIGURATION_FILE_PATH, doc.as<String>())) {
        Logger::error("ConfigurationLoader::save: failed to configuration to file '" CONFIGURATION_FILE_PATH "'");
        return false;
    }

    Logger::info("ConfigurationLoader::save: saved configuration to file '" CONFIGURATION_FILE_PATH "'");
    return true;
}

JsonDocument ConfigurationLoader::getConfigurationFile(const String &path) {
    JsonDocument doc;

    if (!persistentStorage.exists(path)) {
        Logger::error("ConfigurationLoader::getConfigurationFile: file '" + path + "': does not exist", {Logger::Channel::SERIAL_PORT});
        return doc;
    }

    String content = persistentStorage.readFile(path);
    if (content.length() == 0) {
        Logger::error("ConfigurationLoader::getConfigurationFile: file '" + path + "': has zero length", {Logger::Channel::SERIAL_PORT});
        return doc;
    }

    DeserializationError error = deserializeJson(doc, content);
    if (error) {
        Logger::error("ConfigurationLoader::getConfigurationFile: Failed to deserialize file '" + path + "': " +
                           String(error.c_str()), {Logger::Channel::SERIAL_PORT});
    }
    return doc;
}
