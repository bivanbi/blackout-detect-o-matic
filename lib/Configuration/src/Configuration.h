#ifndef BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H
#define BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H

#include "ArduinoJson.h"

#ifndef CONFIGURATION_FILE_PATH
#define CONFIGURATION_FILE_PATH "/config.json"
#endif

#ifndef OVERWRITE_CONFIGURATION
#define OVERWRITE_CONFIGURATION false
#endif

#ifndef LOG_DIRECTORY
#define LOG_DIRECTORY "/log"
#endif

#ifndef SYSTEM_STATUS_FILE_PATH
#define SYSTEM_STATUS_FILE_PATH "/status.json"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "DummyWifiSSID"
#endif

#ifndef WIFI_SECRET
#define WIFI_SECRET "DummyWifiSecret"
#endif

#ifndef NTP_SERVER
#define NTP_SERVER "europe.pool.ntp.org"
#endif

#ifndef NTP_OFFSET
#define NTP_OFFSET 0 // seconds offset from UTC
#endif

#ifndef NTP_UPDATE_INTERVAL
#define NTP_UPDATE_INTERVAL 300000 // milliseconds, e.g. 60000 = 60 seconds
#endif

#ifndef TELNET_SERVER_PORT
#define TELNET_SERVER_PORT 23
#endif

#define CONFIGURATION_FIELD_LOG_DIRECTORY "logDirectory"
#define CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH "systemStatusFilePath"
#define CONFIGURATION_FIELD_WIFI_SSID "wifiSSID"
#define CONFIGURATION_FIELD_WIFI_SECRET "wifiSecret"
#define CONFIGURATION_FIELD_NTP_SERVER "ntpServer"
#define CONFIGURATION_FIELD_NTP_OFFSET "ntpOffset"
#define CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL "ntpUpdateInterval"
#define CONFIGURATION_FIELD_TELNET_SERVER_PORT "telnetServerPort"

class Configuration {
public:
    Configuration() = default;

    explicit Configuration(JsonObject doc);

    String getLogDirectory();

    String getSystemStatusFilePath();

    String getWifiSSID() const;

    String getWifiSecret() const;

    String getNtpServer() const;

    int getNtpOffset() const;

    unsigned int getNtpUpdateInterval() const;

    unsigned int getTelnetServerPort() const;

    JsonDocument toJsonDocument();

private:
    String logDirectory = LOG_DIRECTORY;
    String systemStatusFilePath = SYSTEM_STATUS_FILE_PATH;

    String wifiSSID = WIFI_SSID;
    String wifiSecret = WIFI_SECRET;
    String ntpServer = NTP_SERVER;
    int ntpOffset = NTP_OFFSET;
    int ntpUpdateInterval = NTP_UPDATE_INTERVAL;
    unsigned int telnetServerPort = TELNET_SERVER_PORT;
};

extern Configuration configuration;

#endif //BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H
