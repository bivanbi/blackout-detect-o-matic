#include "Configuration.h"

Configuration::Configuration(JsonObject doc) {
    heartbeatFileLogInterval = doc[CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL];
    heartbeatSerialLogInterval = doc[CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL];
    logFileName = doc[CONFIGURATION_FIELD_LOG_FILE_NAME].as<String>();
    logDirectory = doc[CONFIGURATION_FIELD_LOG_DIRECTORY].as<String>();
    logRotateFileSize = doc[CONFIGURATION_FIELD_LOG_ROTATE_MINIMUM_FILE_SIZE];
    logRotateInterval = doc[CONFIGURATION_FIELD_LOG_ROTATE_TASK_INTERVAL];
    logRotateNumberOfFilesKept = doc[CONFIGURATION_FIELD_LOG_ROTATE_NUMBER_OF_FILES_KEPT];
    ntpOffset = doc[CONFIGURATION_FIELD_NTP_OFFSET];
    ntpServer = doc[CONFIGURATION_FIELD_NTP_SERVER].as<String>();
    ntpUpdateInterval = doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL];
    otaPassword = doc[CONFIGURATION_FIELD_OTA_PASSWORD].as<String>();
    systemStatusFilePath = doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH].as<String>();
    systemStatusSaveInterval = doc[CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL];
    telnetPassword = doc[CONFIGURATION_FIELD_TELNET_PASSWORD].as<String>();
    telnetServerPort = doc[CONFIGURATION_FIELD_TELNET_SERVER_PORT];
    wifiSecret = doc[CONFIGURATION_FIELD_WIFI_SECRET].as<String>();
    wifiSSID = doc[CONFIGURATION_FIELD_WIFI_SSID].as<String>();
}

Configuration::GetResult Configuration::get(const String &key) {
    if (key == CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL) {
        return {OK, String(getHeartbeatFileLogInterval())};
    } else if (key == CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL) {
        return {OK, String(getHeartbeatSerialLogInterval())};
    } else if (key == CONFIGURATION_FIELD_LOG_FILE_NAME) {
        return {OK, getLogFileName()};
    } else if (key == CONFIGURATION_FIELD_LOG_DIRECTORY) {
        return {OK, getLogDirectory()};
    } else if (key == CONFIGURATION_FIELD_LOG_ROTATE_MINIMUM_FILE_SIZE) {
        return {OK, String(getLogRotateMinimumFileSize())};
    } else if (key == CONFIGURATION_FIELD_LOG_ROTATE_TASK_INTERVAL) {
        return {OK, String(getLogRotateTaskInterval())};
    } else if (key == CONFIGURATION_FIELD_LOG_ROTATE_NUMBER_OF_FILES_KEPT) {
        return {OK, String(getLogRotateNumberOfFilesKept())};
    } else if (key == CONFIGURATION_FIELD_NTP_OFFSET) {
        return {OK, String(getNtpOffset())};
    } else if (key == CONFIGURATION_FIELD_NTP_SERVER) {
        return {OK, getNtpServer()};
    } else if (key == CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL) {
        return {OK, String(getNtpUpdateInterval())};
    } else if (key == CONFIGURATION_FIELD_OTA_PASSWORD) {
        return {OK, getOTAPassword()};
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH) {
        return {OK, getSystemStatusFilePath()};
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL) {
        return {OK, String(getSystemStatusSaveInterval())};
    } else if (key == CONFIGURATION_FIELD_TELNET_PASSWORD) {
        return {OK, getTelnetPassword()};
    } else if (key == CONFIGURATION_FIELD_TELNET_SERVER_PORT) {
        return {OK, String(getTelnetServerPort())};
    } else if (key == CONFIGURATION_FIELD_WIFI_SECRET) {
        return {OK, getWifiSecret()};
    } else if (key == CONFIGURATION_FIELD_WIFI_SSID) {
        return {OK, getWifiSSID()};
    }

    return {INVALID_KEY, "unknown key"};
}

unsigned long Configuration::getHeartbeatFileLogInterval() const {
    return heartbeatFileLogInterval;
}

unsigned long Configuration::getHeartbeatSerialLogInterval() const {
    return heartbeatSerialLogInterval;
}

String Configuration::getLogFileName() {
    return logFileName;
}

String Configuration::getLogDirectory() {
    return logDirectory;
}

unsigned long Configuration::getLogRotateMinimumFileSize() const {
    return logRotateFileSize;
}

unsigned long Configuration::getLogRotateTaskInterval() const {
    return logRotateInterval;
}

unsigned long Configuration::getLogRotateNumberOfFilesKept() const {
    return logRotateNumberOfFilesKept;
}

int Configuration::getNtpOffset() const {
    return ntpOffset;
}

String Configuration::getNtpServer() const {
    return ntpServer;
}

unsigned int Configuration::getNtpUpdateInterval() const {
    return ntpUpdateInterval;
}

String Configuration::getOTAPassword() const {
    return otaPassword;
}

String Configuration::getSystemStatusFilePath() {
    return systemStatusFilePath;
}

unsigned long Configuration::getSystemStatusSaveInterval() const {
    return systemStatusSaveInterval;
}

String Configuration::getTelnetPassword() const {
    return telnetPassword;
}

unsigned int Configuration::getTelnetServerPort() const {
    return telnetServerPort;
}

String Configuration::getWifiSecret() const {
    return wifiSecret;
}

String Configuration::getWifiSSID() const {
    return wifiSSID;
}

Configuration::SetResult Configuration::set(const String &key, const String &value) {
    if (key == CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL) {
        setHeartbeatFileLogInterval(value.toInt());
    } else if (key == CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL) {
        setHeartbeatSerialLogInterval(value.toInt());
    } else if (key == CONFIGURATION_FIELD_LOG_FILE_NAME) {
        setLogFileName(value);
    } else if (key == CONFIGURATION_FIELD_LOG_DIRECTORY) {
        setLogDirectory(value);
    } else if (key == CONFIGURATION_FIELD_LOG_ROTATE_MINIMUM_FILE_SIZE) {
        setLogRotateMinimumFileSize(value.toInt());
    } else if (key == CONFIGURATION_FIELD_LOG_ROTATE_TASK_INTERVAL) {
        setLogRotateTaskInterval(value.toInt());
    } else if (key == CONFIGURATION_FIELD_LOG_ROTATE_NUMBER_OF_FILES_KEPT) {
        setLogRotateNumberOfFilesKept(value.toInt());
    } else if (key == CONFIGURATION_FIELD_NTP_SERVER) {
        setNtpServer(value);
    } else if (key == CONFIGURATION_FIELD_NTP_OFFSET) {
        setNtpOffset(value.toInt());
    } else if (key == CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL) {
        setNtpUpdateInterval(value.toInt());
    } else if (key == CONFIGURATION_FIELD_OTA_PASSWORD) {
        setOTAPassword(value);
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH) {
        setSystemStatusFilePath(value);
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL) {
        setSystemStatusSaveInterval(value.toInt());
    } else if (key == CONFIGURATION_FIELD_TELNET_PASSWORD) {
        setTelnetPassword(value);
    } else if (key == CONFIGURATION_FIELD_TELNET_SERVER_PORT) {
        setTelnetServerPort(value.toInt());
    } else if (key == CONFIGURATION_FIELD_WIFI_SECRET) {
        setWifiSecret(value);
    } else if (key == CONFIGURATION_FIELD_WIFI_SSID) {
        setWifiSSID(value);
    } else {
        return {INVALID_KEY, "unknown key"};
    }

    return {OK, "updated"};
}

void Configuration::setHeartbeatFileLogInterval(unsigned long interval) {
    heartbeatFileLogInterval = interval;
}

void Configuration::setHeartbeatSerialLogInterval(unsigned long interval) {
    heartbeatSerialLogInterval = interval;
}

void Configuration::setLogFileName(String fileName) {
    logFileName = std::move(fileName);
}

void Configuration::setLogDirectory(String directory) {
    logDirectory = std::move(directory);
}

void Configuration::setLogRotateMinimumFileSize(unsigned long fileSize) {
    logRotateFileSize = fileSize;
}

void Configuration::setLogRotateTaskInterval(unsigned long interval) {
    logRotateInterval = interval;
}

void Configuration::setLogRotateNumberOfFilesKept(unsigned long numberOfFilesKept) {
    logRotateNumberOfFilesKept = numberOfFilesKept;
}

void Configuration::setNtpOffset(int offset) {
    ntpOffset = offset;
}

void Configuration::setNtpServer(String server) {
    ntpServer = std::move(server);
}

void Configuration::setNtpUpdateInterval(int interval) {
    ntpUpdateInterval = interval;
}

void Configuration::setOTAPassword(String password) {
    otaPassword = std::move(password);
}

void Configuration::setSystemStatusFilePath(String path) {
    systemStatusFilePath = std::move(path);
}

void Configuration::setSystemStatusSaveInterval(unsigned long interval) {
    systemStatusSaveInterval = interval;
}

void Configuration::setTelnetPassword(String password) {
    telnetPassword = std::move(password);
}

void Configuration::setTelnetServerPort(unsigned int port) {
    telnetServerPort = port;
}

void Configuration::setWifiSecret(String secret) {
    wifiSecret = std::move(secret);
}

void Configuration::setWifiSSID(String ssid) {
    wifiSSID = std::move(ssid);
}

JsonDocument Configuration::toJsonDocument() {
    JsonDocument doc;

    doc[CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL] = heartbeatFileLogInterval;
    doc[CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL] = heartbeatSerialLogInterval;
    doc[CONFIGURATION_FIELD_LOG_FILE_NAME] = logFileName;
    doc[CONFIGURATION_FIELD_LOG_DIRECTORY] = logDirectory;
    doc[CONFIGURATION_FIELD_LOG_ROTATE_MINIMUM_FILE_SIZE] = logRotateFileSize;
    doc[CONFIGURATION_FIELD_LOG_ROTATE_TASK_INTERVAL] = logRotateInterval;
    doc[CONFIGURATION_FIELD_LOG_ROTATE_NUMBER_OF_FILES_KEPT] = logRotateNumberOfFilesKept;
    doc[CONFIGURATION_FIELD_NTP_OFFSET] = ntpOffset;
    doc[CONFIGURATION_FIELD_NTP_SERVER] = ntpServer;
    doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL] = ntpUpdateInterval;
    doc[CONFIGURATION_FIELD_OTA_PASSWORD] = otaPassword;
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH] = systemStatusFilePath;
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL] = systemStatusSaveInterval;
    doc[CONFIGURATION_FIELD_TELNET_PASSWORD] = telnetPassword;
    doc[CONFIGURATION_FIELD_TELNET_SERVER_PORT] = telnetServerPort;
    doc[CONFIGURATION_FIELD_WIFI_SECRET] = wifiSecret;
    doc[CONFIGURATION_FIELD_WIFI_SSID] = wifiSSID;

    return doc;
}

Configuration configuration = Configuration();
