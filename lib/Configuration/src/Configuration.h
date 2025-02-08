#ifndef BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H
#define BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H

#include "ArduinoJson.h"

#ifndef CONFIGURATION_FILE_PATH
#define CONFIGURATION_FILE_PATH "/config.json"
#endif

#ifndef OVERWRITE_CONFIGURATION
#define OVERWRITE_CONFIGURATION false
#endif

#ifndef PERIODIC_TASK_INTERVAL
#define PERIODIC_TASK_INTERVAL 1000 // milliseconds, e.g. 1000 = 1 seconds
#endif

#ifndef HEARTBEAT_SERIAL_LOG_INTERVAL
#define HEARTBEAT_SERIAL_LOG_INTERVAL 60 // seconds, e.g. 60 = 60 seconds
#endif

#ifndef HEARTBEAT_FILE_LOG_INTERVAL
#define HEARTBEAT_FILE_LOG_INTERVAL 3600 // seconds, e.g. 3600 = 1 hour
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

#ifndef POWER_DETECTOR_PIN
#define POWER_DETECTOR_PIN 34
#endif

#ifndef CLEAR_ALARM_PIN
#define CLEAR_ALARM_PIN 33
#endif

#ifndef RESET_SYSTEM_STATUS_PIN
#define RESET_SYSTEM_STATUS_PIN 32
#endif

#ifndef ALARM_LED_PIN
#define ALARM_LED_PIN 15
#endif

#define CONFIGURATION_FIELD_LOG_DIRECTORY "logDirectory"
#define CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH "systemStatusFilePath"
#define CONFIGURATION_FIELD_WIFI_SSID "wifiSSID"
#define CONFIGURATION_FIELD_WIFI_SECRET "wifiSecret"
#define CONFIGURATION_FIELD_NTP_SERVER "ntpServer"
#define CONFIGURATION_FIELD_NTP_OFFSET "ntpOffset"
#define CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL "ntpUpdateInterval"
#define CONFIGURATION_FIELD_TELNET_SERVER_PORT "telnetServerPort"
#define CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL "heartbeatSerialLogInterval"
#define CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL "heartbeatFileLogInterval"

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

    unsigned long getHeartbeatSerialLogInterval() const;

    unsigned long getHeartbeatFileLogInterval() const;

    JsonDocument toJsonDocument();

private:
    String logDirectory = LOG_DIRECTORY;
    String systemStatusFilePath = SYSTEM_STATUS_FILE_PATH;

    String wifiSSID = WIFI_SSID;
    String wifiSecret = WIFI_SECRET;

    /**
     * Hostname or IP address of the NTP server
     */
    String ntpServer = NTP_SERVER;

    /**
     * Offset from UTC in seconds
     */
    int ntpOffset = NTP_OFFSET;

    /**
     * NTP update interval in seconds
     */
    int ntpUpdateInterval = NTP_UPDATE_INTERVAL;
    unsigned int telnetServerPort = TELNET_SERVER_PORT;

    /**
     * Interval in seconds for logging the heartbeat to the serial console
     */
    unsigned long heartbeatSerialLogInterval = HEARTBEAT_SERIAL_LOG_INTERVAL;

    /**
     * Interval in seconds for logging the heartbeat to a file
     */
    unsigned long heartbeatFileLogInterval = HEARTBEAT_FILE_LOG_INTERVAL;
};

extern Configuration configuration;

#endif //BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H
