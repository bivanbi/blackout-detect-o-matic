#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

#include "SystemStatus.h"

SystemStatus::SystemStatus(SystemStatus::Data status) {
    data = status;
}

SystemStatus::SystemStatus(JsonObject doc) {
    data.wifiStatus = doc[SYSTEM_STATUS_FIELD_WIFI_STATUS];
    data.isPersistentStorageFailed = doc[SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED];
    data.isTimeSet = doc[SYSTEM_STATUS_FIELD_IS_TIME_SET];
    data.isAlarmActive = doc[SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE];
    data.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(
            doc[SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP].as<JsonObject>());
    data.lastStatusSnapshotTimeStamp = UnixTimeWithMilliSeconds(
            doc[SYSTEM_STATUS_FIELD_LAST_STATUS_SNAPSHOT_TIMESTAMP].as<JsonObject>());
    data.lastRebootTimeStamp = UnixTimeWithMilliSeconds(
            doc[SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP].as<JsonObject>());
    data.lastResetTimeStamp = UnixTimeWithMilliSeconds(doc[SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP].as<JsonObject>());
    data.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(
            doc[SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP].as<JsonObject>());
    data.rebootCount = doc[SYSTEM_STATUS_FIELD_REBOOT_COUNT];
    data.blackoutCount = doc[SYSTEM_STATUS_FIELD_BLACKOUT_COUNT];

    data.lastBlackout = Blackout(doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT].as<JsonObject>());
    data.shortestBlackout = Blackout(doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT].as<JsonObject>());
    data.longestBlackout = Blackout(doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT].as<JsonObject>());
}

void SystemStatus::setWifiStatus(wl_status_t status) {
    data.wifiStatus = status;
}

void SystemStatus::setClockStatus(bool timeSet) {
    data.isTimeSet = timeSet;
}

void SystemStatus::rebootDetected(UnixTimeWithMilliSeconds timeStamp) {
    data.rebootCount++;
    data.isAlarmActive = true;
    data.lastRebootTimeStamp = timeStamp;
}

void SystemStatus::setPowerStatus(UnixTimeWithMilliSeconds timeStamp, bool isPowerOnline) {
    if (isPowerOnline) {
        powerOnDetected(timeStamp);
    } else {
        blackoutDetected(timeStamp);
    }
}

void SystemStatus::powerOnDetected(UnixTimeWithMilliSeconds timeStamp) {
    data.lastBlackout.setEnd(timeStamp);
    if (data.lastBlackout.isValid()) {
        updateLongestBlackout();
        updateShortestBlackout();
    }
}

void SystemStatus::blackoutDetected(UnixTimeWithMilliSeconds timeStamp) {
    data.lastBlackout = Blackout(timeStamp);
    data.isAlarmActive = true;
    data.blackoutCount++;
}

void SystemStatus::updateLongestBlackout() {
    if (!data.longestBlackout.isValid() ||
        data.lastBlackout.getDuration().greaterThan(data.longestBlackout.getDuration())) {
        data.longestBlackout = data.lastBlackout;
    }
}

void SystemStatus::updateShortestBlackout() {
    if (!data.shortestBlackout.isValid() ||
        data.lastBlackout.getDuration().lessThan(data.shortestBlackout.getDuration())) {
        data.shortestBlackout = data.lastBlackout;
    }
}


bool SystemStatus::isPersistentStorageFailed() {
    return data.isPersistentStorageFailed;
}

UnixTimeWithMilliSeconds SystemStatus::getLastPersistentStorageFailedTimeStamp() {
    return data.lastPersistentStorageFailedTimeStamp;
}

void SystemStatus::setLastSnapshotTimeStamp(UnixTimeWithMilliSeconds timeStamp) {
    data.lastStatusSnapshotTimeStamp = timeStamp;
}

bool SystemStatus::isAlarmActive() {
    return data.isAlarmActive;
}

uint32_t SystemStatus::getBlackoutCount() {
    return data.blackoutCount;
}

uint32_t SystemStatus::getRebootCount() {
    return data.rebootCount;
}

bool SystemStatus::isRebootDetected() {
    return data.rebootCount > 0;
}

bool SystemStatus::isWifiConnected() {
    return data.wifiStatus == WL_CONNECTED;
}

bool SystemStatus::isTimeSet() {
    return data.isTimeSet;
}

bool SystemStatus::isBlackoutDetected() {
    return data.blackoutCount > 0;
}

UnixTimeWithMilliSeconds SystemStatus::getLastRebootTimeStamp() {
    return data.lastRebootTimeStamp;
}

SystemStatus::Data SystemStatus::getStatus() {
    return data;
}

Blackout SystemStatus::getLastBlackout() {
    return data.lastBlackout;
}

Blackout SystemStatus::getShortestBlackout() {
    return data.shortestBlackout;
}

Blackout SystemStatus::getLongestBlackout() {
    return data.longestBlackout;
}

void SystemStatus::clearAlarm(UnixTimeWithMilliSeconds timeStamp) {
    data.isAlarmActive = false;
    data.lastAlarmClearedTimeStamp = timeStamp;
}

UnixTimeWithMilliSeconds SystemStatus::getLastAlarmClearedTimeStamp() {
    return data.lastAlarmClearedTimeStamp;
}

UnixTimeWithMilliSeconds SystemStatus::getLastStatusSnapshotTimeStamp() {
    return data.lastStatusSnapshotTimeStamp;
}

UnixTimeWithMilliSeconds SystemStatus::getLastResetTimeStamp() {
    return data.lastResetTimeStamp;
}

void SystemStatus::reset(UnixTimeWithMilliSeconds timeStamp) {
    data.isPersistentStorageFailed = false;
    data.rebootCount = 0;
    data.shortestBlackout = Blackout();
    data.longestBlackout = Blackout();
    data.lastResetTimeStamp = timeStamp;

    if (getLastBlackout().isActive()) {
        data.blackoutCount = 1;
    } else {
        data.lastBlackout = Blackout();
        data.blackoutCount = 0;
        data.isAlarmActive = false;
    }

}

JsonDocument SystemStatus::toJsonDocument() {
    JsonDocument doc;
    doc[SYSTEM_STATUS_FIELD_WIFI_STATUS] = data.wifiStatus;
    doc[SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED] = data.isPersistentStorageFailed;
    doc[SYSTEM_STATUS_FIELD_IS_TIME_SET] = data.isTimeSet;
    doc[SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE] = data.isAlarmActive;
    doc[SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP] = data.lastPersistentStorageFailedTimeStamp.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_STATUS_SNAPSHOT_TIMESTAMP] = data.lastStatusSnapshotTimeStamp.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP] = data.lastRebootTimeStamp.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP] = data.lastResetTimeStamp.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP] = data.lastAlarmClearedTimeStamp.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_REBOOT_COUNT] = data.rebootCount;
    doc[SYSTEM_STATUS_FIELD_BLACKOUT_COUNT] = data.blackoutCount;

    doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT] = data.lastBlackout.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT] = data.shortestBlackout.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT] = data.longestBlackout.toJsonDocument();
    return doc;
}

JsonDocument SystemStatus::toHumanReadableJsonDocument() {
    JsonDocument doc;

    doc[SYSTEM_STATUS_FIELD_IS_TIME_SET] = data.isTimeSet;
    doc[SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE] = data.isAlarmActive;
    doc[SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED] = data.isPersistentStorageFailed;
    doc[SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP] = data.lastPersistentStorageFailedTimeStamp.getFormattedTime() + "." + String(data.lastPersistentStorageFailedTimeStamp.getMilliSeconds());
    doc[SYSTEM_STATUS_FIELD_LAST_STATUS_SNAPSHOT_TIMESTAMP] = data.lastStatusSnapshotTimeStamp.getFormattedTime() + "." + String(data.lastStatusSnapshotTimeStamp.getMilliSeconds());
    doc[SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP] = data.lastRebootTimeStamp.getFormattedTime() + "." + String(data.lastRebootTimeStamp.getMilliSeconds());
    doc[SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP] = data.lastResetTimeStamp.getFormattedTime() + "." + String(data.lastResetTimeStamp.getMilliSeconds());
    doc[SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP] = data.lastAlarmClearedTimeStamp.getFormattedTime() + "." + String(data.lastAlarmClearedTimeStamp.getMilliSeconds());
    doc[SYSTEM_STATUS_FIELD_REBOOT_COUNT] = data.rebootCount;
    doc[SYSTEM_STATUS_FIELD_BLACKOUT_COUNT] = data.blackoutCount;

    doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT] = data.lastBlackout.toHumanReadableJsonDocument();
    doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT] = data.shortestBlackout.toHumanReadableJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT] = data.longestBlackout.toHumanReadableJsonDocument();

    return doc;
}

void SystemStatus::persistentStorageFailed(UnixTimeWithMilliSeconds timeStamp) {
    data.isAlarmActive = true;
    data.isPersistentStorageFailed = true;
    data.lastPersistentStorageFailedTimeStamp = timeStamp;
}

bool SystemStatus::equals(SystemStatus other) {
    return toJsonDocument().as<String>().equals(other.toJsonDocument().as<String>());
}

SystemStatus systemStatus = SystemStatus();

#pragma clang diagnostic pop