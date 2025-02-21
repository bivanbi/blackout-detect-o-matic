#ifndef BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H
#define BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H

#include <ArduinoJson.h>

#ifndef CONFIGURATION_FILE_PATH
#define CONFIGURATION_FILE_PATH "/config.json"
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

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME "blackout-detect-o-matic.log"
#endif

#ifndef LOG_ROTATE_MINIMUM_FILE_SIZE
#define LOG_ROTATE_MINIMUM_FILE_SIZE 1048576 // bytes, e.g. 1048576 = 1 MB
#endif

#ifndef LOG_ROTATE_TASK_INTERVAL
#define LOG_ROTATE_TASK_INTERVAL 86400 // seconds, e.g. 86400 = 1 day
#endif

#ifndef LOG_ROTATE_NUMBER_OF_FILES_KEPT
#define LOG_ROTATE_NUMBER_OF_FILES_KEPT 60 // number of files kept above active log file
#endif

#ifndef SYSTEM_STATUS_FILE_PATH
#define SYSTEM_STATUS_FILE_PATH "/status.json"
#endif

#ifndef SYSTEM_STATUS_SAVE_INTERVAL
#define SYSTEM_STATUS_SAVE_INTERVAL 3600 // seconds
#endif

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

#ifndef WIFI_SECRET
#define WIFI_SECRET ""
#endif

#ifndef NTP_SERVER
#define NTP_SERVER "europe.pool.ntp.org"
#endif

#ifndef NTP_OFFSET
#define NTP_OFFSET 0 // seconds offset from UTC
#endif

#ifndef NTP_UPDATE_INTERVAL
#define NTP_UPDATE_INTERVAL 300 // seconds
#endif

#ifndef OTA_PASSWORD
#define OTA_PASSWORD ""
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
#define ALARM_LED_PIN 4
#endif

#define CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL "heartbeatFileLogInterval"
#define CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL "heartbeatSerialLogInterval"
#define CONFIGURATION_FIELD_LOG_FILE_NAME "logFileName"
#define CONFIGURATION_FIELD_LOG_DIRECTORY "logDirectory"
#define CONFIGURATION_FIELD_LOG_ROTATE_MINIMUM_FILE_SIZE "logRotateMinimumFileSize"
#define CONFIGURATION_FIELD_LOG_ROTATE_TASK_INTERVAL "logRotateTaskInterval"
#define CONFIGURATION_FIELD_LOG_ROTATE_NUMBER_OF_FILES_KEPT "logRotateNumberOfFilesKept"
#define CONFIGURATION_FIELD_NTP_OFFSET "ntpOffset"
#define CONFIGURATION_FIELD_NTP_SERVER "ntpServer"
#define CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL "ntpUpdateInterval"
#define CONFIGURATION_FIELD_OTA_PASSWORD "otaPassword"
#define CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH "systemStatusFilePath"
#define CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL "systemStatusSaveInterval"
#define CONFIGURATION_FIELD_TELNET_SERVER_PORT "telnetServerPort"
#define CONFIGURATION_FIELD_WIFI_SECRET "wifiSecret"
#define CONFIGURATION_FIELD_WIFI_SSID "wifiSSID"

class Configuration {
public:
    enum ResultCode {
        OK,
        INVALID_KEY
    };

    struct SetResult {
        ResultCode code;
        String message;
    };

    struct GetResult {
        ResultCode code;
        String value;
    };

    Configuration() = default;

    explicit Configuration(JsonObject doc);

    // Getters

    GetResult get(const String &key);

    unsigned long getHeartbeatFileLogInterval() const;

    unsigned long getHeartbeatSerialLogInterval() const;

    String getLogFileName();

    String getLogDirectory();

    /**
     * Get the minimum file size in bytes before rotating the log file.
     *
     * @return minimum log file size in bytes
     */
    unsigned long getLogRotateMinimumFileSize() const;

    /**
     * Get the interval in seconds for running the logrotate task.
     * Rotation will only occur, if the active log file size reaches the minimum file size.
     *
     * @return task interval in seconds
     */
    unsigned long getLogRotateTaskInterval() const;

    /**
     * Get the number of log files (above active log file) to keep before deleting the oldest.
     *
     * @return number of rotated log files to be kept
     */
    unsigned long getLogRotateNumberOfFilesKept() const;

    int getNtpOffset() const;

    String getNtpServer() const;

    unsigned int getNtpUpdateInterval() const;

    String getOTAPassword() const;

    String getSystemStatusFilePath();

    unsigned long getSystemStatusSaveInterval() const;

    unsigned int getTelnetServerPort() const;

    String getWifiSecret() const;

    String getWifiSSID() const;

    // Setters

    SetResult set(const String &key, const String &value);

    void setHeartbeatFileLogInterval(unsigned long interval);

    void setHeartbeatSerialLogInterval(unsigned long interval);

    void setLogFileName(String fileName);

    void setLogDirectory(String directory);

    /**
     * Set the minimum file size in bytes before rotating the log file.
     *
     * @param fileSize in bytes
     */
    void setLogRotateMinimumFileSize(unsigned long fileSize);

    void setLogRotateTaskInterval(unsigned long interval);

    /**
     * Set the number of log files (above active log file) to keep before deleting the oldest.
     * For example, if set to 7, then there will be the active log file plus 7 rotated log files.
     *
     * @param numberOfFilesKept number of rotated log files to be kept
     */
    void setLogRotateNumberOfFilesKept(unsigned long numberOfFilesKept);

    void setNtpOffset(int offset);

    void setNtpServer(String server);

    void setNtpUpdateInterval(int interval);

    void setOTAPassword(String password);

    void setSystemStatusFilePath(String path);

    void setSystemStatusSaveInterval(unsigned long interval);

    void setTelnetServerPort(unsigned int port);

    void setWifiSecret(String secret);

    void setWifiSSID(String ssid);

    JsonDocument toJsonDocument();

private:
    /**
     * Interval in seconds for logging the heartbeat to a file
     */
    unsigned long heartbeatFileLogInterval = HEARTBEAT_FILE_LOG_INTERVAL;

    /**
     * Interval in seconds for logging the heartbeat to the serial console
     */
    unsigned long heartbeatSerialLogInterval = HEARTBEAT_SERIAL_LOG_INTERVAL;

    String logFileName = LOG_FILE_NAME;
    String logDirectory = LOG_DIRECTORY;
    unsigned long logRotateFileSize = LOG_ROTATE_MINIMUM_FILE_SIZE;
    unsigned long logRotateInterval = LOG_ROTATE_TASK_INTERVAL;
    unsigned long logRotateNumberOfFilesKept = LOG_ROTATE_NUMBER_OF_FILES_KEPT;

    /**
     * Offset from UTC in seconds
     */
    int ntpOffset = NTP_OFFSET;

    /**
     * Hostname or IP address of the NTP server
     */
    String ntpServer = NTP_SERVER;

    /**
     * NTP update interval in seconds
     */
    int ntpUpdateInterval = NTP_UPDATE_INTERVAL;

    String otaPassword = OTA_PASSWORD;

    unsigned int telnetServerPort = TELNET_SERVER_PORT;


    String systemStatusFilePath = SYSTEM_STATUS_FILE_PATH;

    /**
     * Interval in seconds for saving the system status to a file
     */
    unsigned long systemStatusSaveInterval = SYSTEM_STATUS_SAVE_INTERVAL;

    String wifiSecret = WIFI_SECRET;
    String wifiSSID = WIFI_SSID;
};

extern Configuration configuration;

#endif //BLACKOUT_DETECT_O_MATIC_CONFIGURATION_H
