#ifndef BLACKOUT_DETECT_O_MATIC_SYSTEM_STATUS_H
#define BLACKOUT_DETECT_O_MATIC_SYSTEM_STATUS_H

#include <WiFiType.h>
#include <ArduinoJson.h>

#include "Blackout.h"

#define SYSTEM_STATUS_FIELD_WIFI_STATUS "wifiStatus"
#define SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED "isPermanentStorageFailed"
#define SYSTEM_STATUS_FIELD_IS_TIME_SET "isTimeSet"
#define SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE "isAlarmActive"
#define SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP "lastPermanentStorageFailedTimestamp"
#define SYSTEM_STATUS_FIELD_LAST_STATUS_TIMESTAMP "lastStatusTimestamp"
#define SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP "lastRebootTimestamp"
#define SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP "lastResetTimestamp"
#define SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP "lastAlarmClearedTimestamp"
#define SYSTEM_STATUS_FIELD_REBOOT_COUNT "rebootCount"
#define SYSTEM_STATUS_FIELD_BLACKOUT_COUNT "blackoutCount"
#define SYSTEM_STATUS_FIELD_LAST_BLACKOUT "lastBlackout"
#define SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT "shortestBlackout"
#define SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT "longestBlackout"

class SystemStatus {
public:
    struct Data {
        wl_status_t wifiStatus = WL_DISCONNECTED;

        bool isTimeSet = false;
        bool isAlarmActive = false;
        bool isPersistentStorageFailed = false;

        UnixTimeWithMilliSeconds lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds();
        UnixTimeWithMilliSeconds lastStatusSnapshotTimeStamp = UnixTimeWithMilliSeconds();
        UnixTimeWithMilliSeconds lastRebootTimeStamp = UnixTimeWithMilliSeconds();
        UnixTimeWithMilliSeconds lastResetTimeStamp = UnixTimeWithMilliSeconds();
        UnixTimeWithMilliSeconds lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds();

        uint32_t rebootCount = 0;
        uint32_t blackoutCount = 0;

        Blackout lastBlackout;
        Blackout shortestBlackout;
        Blackout longestBlackout;
    };

    const static bool POWER_ONLINE = true;
    const static bool POWER_OFFLINE = false;
    const static long UNDEFINED = -1;

    SystemStatus() = default;

    explicit SystemStatus(Data status);

    explicit SystemStatus(JsonDocument doc);

    Data getStatus();

    void persistentStorageFailed(UnixTimeWithMilliSeconds timeStamp);

    void rebootDetected(UnixTimeWithMilliSeconds timeStamp);

    UnixTimeWithMilliSeconds getLastPersistentStorageFailedTimeStamp();

    UnixTimeWithMilliSeconds getLastStatusSnapshotTimeStamp();

    UnixTimeWithMilliSeconds getLastRebootTimeStamp();

    UnixTimeWithMilliSeconds getLastResetTimeStamp();

    UnixTimeWithMilliSeconds getLastAlarmClearedTimeStamp();

    void setWifiStatus(wl_status_t status);

    void setClockStatus(bool timeSet);

    void setLastSnapshotTimeStamp(UnixTimeWithMilliSeconds timeStamp);

    void setPowerStatus(UnixTimeWithMilliSeconds timeStamp, bool isPowerOnline);

    void clearAlarm(UnixTimeWithMilliSeconds timeStamp); // clear current alarm without resetting status
    void
    reset(UnixTimeWithMilliSeconds timeStamp); // clear blackout and reboot counters, shortest / longest blackout. Does not clear last timestamps

    bool isAlarmActive();

    bool isRebootDetected();

    bool isBlackoutDetected();

    bool isWifiConnected();

    bool isTimeSet();

    bool isPersistentStorageFailed();

    uint32_t getBlackoutCount();

    uint32_t getRebootCount();

    Blackout getLastBlackout();

    Blackout getShortestBlackout();

    Blackout getLongestBlackout();

    JsonDocument toJsonDocument();

private:
    Data data;

    void blackoutDetected(UnixTimeWithMilliSeconds timeStamp);

    void powerOnDetected(UnixTimeWithMilliSeconds timeStamp);

    void updateLongestBlackout();

    void updateShortestBlackout();
};

extern SystemStatus systemStatus;
#endif //BLACKOUT_DETECT_O_MATIC_SYSTEM_STATUS_H
