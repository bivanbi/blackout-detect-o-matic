#include "Configuration.h"

Configuration::Configuration(JsonObject doc) {
    logDirectory = doc[CONFIGURATION_FIELD_LOG_DIRECTORY].as<String>();
    systemStatusFilePath = doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH].as<String>();
    wifiSSID = doc[CONFIGURATION_FIELD_WIFI_SSID].as<String>();
    wifiSecret = doc[CONFIGURATION_FIELD_WIFI_SECRET].as<String>();
    ntpServer = doc[CONFIGURATION_FIELD_NTP_SERVER].as<String>();
    ntpOffset = doc[CONFIGURATION_FIELD_NTP_OFFSET];
    ntpUpdateInterval = doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL];
}

String Configuration::getLogDirectory() {
    return logDirectory;
}

String Configuration::getSystemStatusFilePath() {
    return systemStatusFilePath;
}

String Configuration::getWifiSSID() const {
    return wifiSSID;
}

String Configuration::getWifiSecret() const {
    return wifiSecret;
}

String Configuration::getNtpServer() const {
    return ntpServer;
}

int Configuration::getNtpOffset() const {
    return ntpOffset;
}

unsigned int Configuration::getNtpUpdateInterval() const {
    return ntpUpdateInterval;
}

JsonDocument Configuration::toJsonDocument() {
    JsonDocument doc;

    doc[CONFIGURATION_FIELD_LOG_DIRECTORY] = logDirectory;
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH] = systemStatusFilePath;
    doc[CONFIGURATION_FIELD_WIFI_SSID] = wifiSSID;
    doc[CONFIGURATION_FIELD_WIFI_SECRET] = wifiSecret;
    doc[CONFIGURATION_FIELD_NTP_SERVER] = ntpServer;
    doc[CONFIGURATION_FIELD_NTP_OFFSET] = ntpOffset;
    doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL] = ntpUpdateInterval;

    return doc;
}

Configuration configuration = Configuration();
