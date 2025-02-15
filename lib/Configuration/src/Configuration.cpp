#include "Configuration.h"

Configuration::Configuration(JsonObject doc) {
    heartbeatFileLogInterval = doc[CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL];
    heartbeatSerialLogInterval = doc[CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL];
    logFileName = doc[CONFIGURATION_FIELD_LOG_FILE_NAME].as<String>();
    logDirectory = doc[CONFIGURATION_FIELD_LOG_DIRECTORY].as<String>();
    ntpOffset = doc[CONFIGURATION_FIELD_NTP_OFFSET];
    ntpServer = doc[CONFIGURATION_FIELD_NTP_SERVER].as<String>();
    ntpUpdateInterval = doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL];
    systemStatusFilePath = doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH].as<String>();
    systemStatusSaveInterval = doc[CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL];
    telnetServerPort = doc[CONFIGURATION_FIELD_TELNET_SERVER_PORT];
    wifiSecret = doc[CONFIGURATION_FIELD_WIFI_SECRET].as<String>();
    wifiSSID = doc[CONFIGURATION_FIELD_WIFI_SSID].as<String>();
}

Configuration::GetResult Configuration::get(const String& key) {
    if (key == CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL) {
        return {OK, String(getHeartbeatFileLogInterval())};
    } else if (key == CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL) {
        return {OK, String(getHeartbeatSerialLogInterval())};
    } else if (key == CONFIGURATION_FIELD_LOG_FILE_NAME) {
        return {OK, getLogFileName()};
    } else if (key == CONFIGURATION_FIELD_LOG_DIRECTORY) {
        return {OK, getLogDirectory()};
    } else if (key == CONFIGURATION_FIELD_NTP_OFFSET) {
        return {OK, String(getNtpOffset())};
    } else if (key == CONFIGURATION_FIELD_NTP_SERVER) {
        return {OK, getNtpServer()};
    } else if (key == CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL) {
        return {OK, String(getNtpUpdateInterval())};
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH) {
        return {OK, getSystemStatusFilePath()};
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL) {
        return {OK, String(getSystemStatusSaveInterval())};
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

int Configuration::getNtpOffset() const {
    return ntpOffset;
}

String Configuration::getNtpServer() const {
    return ntpServer;
}

unsigned int Configuration::getNtpUpdateInterval() const {
    return ntpUpdateInterval;
}

String Configuration::getSystemStatusFilePath() {
    return systemStatusFilePath;
}

unsigned long Configuration::getSystemStatusSaveInterval() const {
    return systemStatusSaveInterval;
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

Configuration::SetResult Configuration::set(const String& key, const String& value) {
    if (key == CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL) {
        heartbeatFileLogInterval = value.toInt();
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL) {
        heartbeatSerialLogInterval = value.toInt();
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_LOG_FILE_NAME) {
        setLogFileName(value);
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_LOG_DIRECTORY) {
        setLogDirectory(value);
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_NTP_SERVER) {
        ntpServer = value;
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_NTP_OFFSET) {
        ntpOffset = value.toInt();
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL) {
        ntpUpdateInterval = value.toInt();
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH) {
        setSystemStatusFilePath(value);
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL) {
        systemStatusSaveInterval = value.toInt();
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_TELNET_SERVER_PORT) {
        telnetServerPort = value.toInt();
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_WIFI_SECRET) {
        wifiSecret = value;
        return {OK, "updated"};
    } else if (key == CONFIGURATION_FIELD_WIFI_SSID) {
        setWifiSSID(value);
        return {OK, "updated"};
    }

    return {INVALID_KEY, "unknown key"};
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

void Configuration::setNtpOffset(int offset) {
    ntpOffset = offset;
}

void Configuration::setNtpServer(String server) {
    ntpServer = std::move(server);
}

void Configuration::setNtpUpdateInterval(int interval) {
    ntpUpdateInterval = interval;
}

void Configuration::setSystemStatusFilePath(String path) {
    systemStatusFilePath = std::move(path);
}

void Configuration::setSystemStatusSaveInterval(unsigned long interval) {
    systemStatusSaveInterval = interval;
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
    doc[CONFIGURATION_FIELD_NTP_OFFSET] = ntpOffset;
    doc[CONFIGURATION_FIELD_NTP_SERVER] = ntpServer;
    doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL] = ntpUpdateInterval;
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH] = systemStatusFilePath;
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL] = systemStatusSaveInterval;
    doc[CONFIGURATION_FIELD_TELNET_SERVER_PORT] = telnetServerPort;
    doc[CONFIGURATION_FIELD_WIFI_SECRET] = wifiSecret;
    doc[CONFIGURATION_FIELD_WIFI_SSID] = wifiSSID;

    return doc;
}

Configuration configuration = Configuration();
