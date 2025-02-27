#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_SystemStatus.h"

void setUp(void) {
    systemStatus = SystemStatus(); // make sure we are using a pristine instance for each test
}

void tearDown(void) {
    // clean stuff up here
}

void test_constructor_default() {
    Blackout initialBlackout = Blackout();
    UnixTimeWithMilliSeconds nullTime = UnixTimeWithMilliSeconds();

    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
    TEST_ASSERT_FALSE(systemStatus.isRebootDetected());
    TEST_ASSERT_FALSE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_FALSE(systemStatus.isTimeSet());
    TEST_ASSERT_FALSE(systemStatus.isWifiConnected());
    TEST_ASSERT_FALSE(systemStatus.isPersistentStorageFailed());

    TEST_ASSERT_EQUAL(0, systemStatus.getRebootCount());
    TEST_ASSERT_EQUAL(0, systemStatus.getBlackoutCount());
    TEST_ASSERT_TRUE(nullTime.equals(systemStatus.getLastPersistentStorageFailedTimeStamp()));
    TEST_ASSERT_TRUE(nullTime.equals(systemStatus.getlastStatusSaveTimeStamp()));
    TEST_ASSERT_TRUE(nullTime.equals(systemStatus.getLastRebootTimeStamp()));
    TEST_ASSERT_TRUE(nullTime.equals(systemStatus.getLastAlarmClearedTimeStamp()));
    TEST_ASSERT_TRUE(nullTime.equals(systemStatus.getLastResetTimeStamp()));
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getLastBlackout()));
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getShortestBlackout()));
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getLongestBlackout()));
}

void test_constructor_withStatusStruct() {
    SystemStatus::Data status;
    status.wifiStatus = WL_CONNECTED;
    status.isPersistentStorageFailed = true;
    status.isTimeSet = true;
    status.isAlarmActive = true;
    status.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(50, 100);
    status.lastStatusSaveTimeStamp = UnixTimeWithMilliSeconds(150, 200);
    status.lastRebootTimeStamp = UnixTimeWithMilliSeconds(250, 300);
    status.lastResetTimeStamp = UnixTimeWithMilliSeconds(350, 400);
    status.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(450, 500);
    status.rebootCount = 5;
    status.blackoutCount = 3;
    status.lastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 150), UnixTimeWithMilliSeconds(200, 250));
    status.shortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 350), UnixTimeWithMilliSeconds(400, 450));
    status.longestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 550), UnixTimeWithMilliSeconds(600, 650));

    systemStatus = SystemStatus(status);

    TEST_ASSERT_TRUE(statusEquals(status, systemStatus.getStatus()));
}

void test_constructor_withJsonDocument() {
    JsonDocument doc;
    Blackout expectedLastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 150), UnixTimeWithMilliSeconds(210, 250));
    Blackout expectedShortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 350),
                                                 UnixTimeWithMilliSeconds(430, 450));
    Blackout expectedLongestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 550), UnixTimeWithMilliSeconds(650, 650));

    doc[SYSTEM_STATUS_FIELD_WIFI_STATUS] = WL_CONNECTED;
    doc[SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED] = true;
    doc[SYSTEM_STATUS_FIELD_IS_TIME_SET] = true;
    doc[SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE] = true;
    doc[SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP] = UnixTimeWithMilliSeconds(50,
                                                                                                 100).toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_STATUS_SAVE_TIMESTAMP] = UnixTimeWithMilliSeconds(500, 100).toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP] = UnixTimeWithMilliSeconds(600, 100).toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP] = UnixTimeWithMilliSeconds(700, 100).toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP] = UnixTimeWithMilliSeconds(800, 100).toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_REBOOT_COUNT] = 5;
    doc[SYSTEM_STATUS_FIELD_BLACKOUT_COUNT] = 3;

    doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT] = expectedLastBlackout.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT] = expectedShortestBlackout.toJsonDocument();
    doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT] = expectedLongestBlackout.toJsonDocument();

    systemStatus = SystemStatus(doc.as<JsonObject>());

    SystemStatus::Data actualStatus = systemStatus.getStatus();

    TEST_ASSERT_EQUAL(WL_CONNECTED, actualStatus.wifiStatus);
    TEST_ASSERT_TRUE(actualStatus.isPersistentStorageFailed);
    TEST_ASSERT_TRUE(actualStatus.isTimeSet);
    TEST_ASSERT_TRUE(actualStatus.isAlarmActive);
    TEST_ASSERT_EQUAL(50, actualStatus.lastPersistentStorageFailedTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(500, actualStatus.lastStatusSaveTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(600, actualStatus.lastRebootTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(700, actualStatus.lastResetTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(800, actualStatus.lastAlarmClearedTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(5, actualStatus.rebootCount);
    TEST_ASSERT_EQUAL(3, actualStatus.blackoutCount);

    TEST_ASSERT_TRUE(expectedLastBlackout.equals(actualStatus.lastBlackout));
    TEST_ASSERT_TRUE(expectedShortestBlackout.equals(actualStatus.shortestBlackout));
    TEST_ASSERT_TRUE(expectedLongestBlackout.equals(actualStatus.longestBlackout));
}

void test_getStatus_initial() {
    SystemStatus::Data status = systemStatus.getStatus();
    Blackout initialBlackout = Blackout();

    TEST_ASSERT_EQUAL(WL_DISCONNECTED, status.wifiStatus);
    TEST_ASSERT_FALSE(status.isPersistentStorageFailed);
    TEST_ASSERT_FALSE(status.isTimeSet);
    TEST_ASSERT_FALSE(status.isAlarmActive);
    TEST_ASSERT_EQUAL(0, status.rebootCount);
    TEST_ASSERT_EQUAL(0, status.blackoutCount);
    TEST_ASSERT_EQUAL(0, status.lastPersistentStorageFailedTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(0, status.lastStatusSaveTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(0, status.lastRebootTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(0, status.lastResetTimeStamp.getUnixTime());
    TEST_ASSERT_EQUAL(0, status.lastAlarmClearedTimeStamp.getUnixTime());

    TEST_ASSERT_TRUE(initialBlackout.equals(status.lastBlackout));
    TEST_ASSERT_TRUE(initialBlackout.equals(status.shortestBlackout));
    TEST_ASSERT_TRUE(initialBlackout.equals(status.longestBlackout));
}

void test_rebootDetected() {
    UnixTimeWithMilliSeconds expectedTimeStamp = UnixTimeWithMilliSeconds(123456789, 321);
    systemStatus.rebootDetected(expectedTimeStamp);

    TEST_ASSERT_EQUAL(1, systemStatus.getRebootCount());
    TEST_ASSERT_TRUE(systemStatus.isAlarmActive());
    TEST_ASSERT_TRUE(systemStatus.isRebootDetected());
    TEST_ASSERT_TRUE(expectedTimeStamp.equals(systemStatus.getLastRebootTimeStamp()));
}

void test_persistentStorageFailed() {
    UnixTimeWithMilliSeconds expectedTimeStamp = UnixTimeWithMilliSeconds(123456789, 321);
    systemStatus.persistentStorageFailed(expectedTimeStamp);

    TEST_ASSERT_TRUE(systemStatus.isAlarmActive());
    TEST_ASSERT_TRUE(systemStatus.isPersistentStorageFailed());
    TEST_ASSERT_TRUE(expectedTimeStamp.equals(systemStatus.getLastPersistentStorageFailedTimeStamp()));
}

void test_getLastBlackout_initial() {
    Blackout blackout = systemStatus.getLastBlackout();
    TEST_ASSERT_EQUAL(0, blackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(0, blackout.getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(0, blackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(0, blackout.getEnd().getMilliSeconds());
}

void test_setPowerStatus_withPowerGoingOffline() {
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(123456789, 321), SystemStatus::POWER_OFFLINE);

    TEST_ASSERT_TRUE(systemStatus.isAlarmActive());
    TEST_ASSERT_TRUE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_EQUAL(1, systemStatus.getBlackoutCount());
    TEST_ASSERT_EQUAL(123456789, systemStatus.getLastBlackout().getStart().getUnixTime());
    TEST_ASSERT_EQUAL(321, systemStatus.getLastBlackout().getStart().getMilliSeconds());
}

void test_setPowerStatus_withPowerGoingOnline() {
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(100, 151), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(200, 251), SystemStatus::POWER_ONLINE);

    Blackout lastBlackout = systemStatus.getLastBlackout();

    TEST_ASSERT_FALSE(lastBlackout.isActive());
    TEST_ASSERT_TRUE(systemStatus.isAlarmActive()); // alarm should stay to inform user of the blackout
    TEST_ASSERT_TRUE(systemStatus.isBlackoutDetected()); // should stay true until user resets it
    TEST_ASSERT_EQUAL(1, systemStatus.getBlackoutCount());
    TEST_ASSERT_EQUAL(100, lastBlackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(151, lastBlackout.getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(200, lastBlackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(251, lastBlackout.getEnd().getMilliSeconds());
    TEST_ASSERT_EQUAL(1, lastBlackout.getDuration().getSign());
    TEST_ASSERT_EQUAL(100, lastBlackout.getDuration().getSeconds());
    TEST_ASSERT_EQUAL(100, lastBlackout.getDuration().getMilliSeconds());
    TEST_ASSERT_TRUE(lastBlackout.equals(systemStatus.getLongestBlackout()));
    TEST_ASSERT_TRUE(lastBlackout.equals(systemStatus.getShortestBlackout()));
}

void test_setPowerStatus_withMultipleBlackouts() {
    Blackout shortest = Blackout(UnixTimeWithMilliSeconds(100, 110), UnixTimeWithMilliSeconds(100, 120));
    Blackout middle = Blackout(UnixTimeWithMilliSeconds(200, 210), UnixTimeWithMilliSeconds(210, 220));
    Blackout longest = Blackout(UnixTimeWithMilliSeconds(300, 320), UnixTimeWithMilliSeconds(330, 340));

    systemStatus.setPowerStatus(longest.getStart(), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(longest.getEnd(), SystemStatus::POWER_ONLINE);

    systemStatus.setPowerStatus(shortest.getStart(), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(shortest.getEnd(), SystemStatus::POWER_ONLINE);
    TEST_ASSERT_EQUAL(2, systemStatus.getBlackoutCount());

    systemStatus.setPowerStatus(middle.getStart(), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(middle.getEnd(), SystemStatus::POWER_ONLINE);

    TEST_ASSERT_TRUE(systemStatus.isAlarmActive()); // alarm should stay to inform user of the blackout
    TEST_ASSERT_EQUAL(3, systemStatus.getBlackoutCount());

    TEST_ASSERT_TRUE(shortest.equals(systemStatus.getShortestBlackout()));
    TEST_ASSERT_TRUE(longest.equals(systemStatus.getLongestBlackout()));
    TEST_ASSERT_TRUE(middle.equals(systemStatus.getLastBlackout()));
}

void test_setPowerStatus_withPowerGoingOffline_duringOngoingBlackout() {
    // SystemStatus purely to store system status. It is up to the callers to decide what happens if there is
    // a blackout detected while another one is still ongoing.
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(100, 101), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(200, 210), SystemStatus::POWER_OFFLINE);

    TEST_ASSERT_EQUAL(200, systemStatus.getLastBlackout().getStart().getUnixTime());
    TEST_ASSERT_EQUAL(210, systemStatus.getLastBlackout().getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(2, systemStatus.getBlackoutCount());
}

void test_setPowerStatus_withPowerGoingOnline_withoutActiveBlackout() {
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(100, 123), SystemStatus::POWER_ONLINE);
    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
    TEST_ASSERT_FALSE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_EQUAL(0, systemStatus.getBlackoutCount());
}

void test_clearAlarm() {
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(100, 100));
    systemStatus.clearAlarm(UnixTimeWithMilliSeconds(200, 200));

    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
    TEST_ASSERT_EQUAL(200, systemStatus.getLastAlarmClearedTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(200, systemStatus.getLastAlarmClearedTimeStamp().getMilliSeconds());
}

void test_reset_withNoActiveBlackout() {
    Blackout initialBlackout = Blackout();

    systemStatus.persistentStorageFailed(UnixTimeWithMilliSeconds(50, 50));
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(100, 100));
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(200, 200), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(300, 300), SystemStatus::POWER_ONLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(400, 400), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(500, 500), SystemStatus::POWER_ONLINE);
    systemStatus.statusSaved(UnixTimeWithMilliSeconds(600, 600));
    systemStatus.clearAlarm(UnixTimeWithMilliSeconds(700, 700));

    systemStatus.reset(UnixTimeWithMilliSeconds(800, 800));

    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
    TEST_ASSERT_FALSE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_FALSE(systemStatus.isRebootDetected());

    TEST_ASSERT_EQUAL(0, systemStatus.getRebootCount());
    TEST_ASSERT_EQUAL(0, systemStatus.getBlackoutCount());
    TEST_ASSERT_EQUAL(50, systemStatus.getLastPersistentStorageFailedTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(100, systemStatus.getLastRebootTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(600, systemStatus.getlastStatusSaveTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(700, systemStatus.getLastAlarmClearedTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(800, systemStatus.getLastResetTimeStamp().getUnixTime());

    TEST_ASSERT_FALSE(systemStatus.isPersistentStorageFailed());
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getLastBlackout()));
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getShortestBlackout()));
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getLongestBlackout()));
}

void test_reset_withActiveBlackout() {
    Blackout initialBlackout = Blackout();

    systemStatus.persistentStorageFailed(UnixTimeWithMilliSeconds(50, 50));
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(100, 100));
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(200, 200), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(300, 300), SystemStatus::POWER_ONLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(400, 400), SystemStatus::POWER_OFFLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(500, 500), SystemStatus::POWER_ONLINE);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(550, 550), SystemStatus::POWER_OFFLINE);
    systemStatus.statusSaved(UnixTimeWithMilliSeconds(600, 600));

    systemStatus.reset(UnixTimeWithMilliSeconds(800, 800));

    TEST_ASSERT_TRUE(systemStatus.isAlarmActive());
    TEST_ASSERT_TRUE(systemStatus.isBlackoutDetected());

    TEST_ASSERT_EQUAL(1, systemStatus.getBlackoutCount());
    TEST_ASSERT_EQUAL(800, systemStatus.getLastResetTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(550, systemStatus.getLastBlackout().getStart().getUnixTime());
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getShortestBlackout()));
    TEST_ASSERT_TRUE(initialBlackout.equals(systemStatus.getLongestBlackout()));
}

void test_lastStatusSaveTimestamp() {
    TEST_ASSERT_EQUAL(0, systemStatus.getlastStatusSaveTimeStamp().getUnixTime());
    systemStatus.statusSaved(UnixTimeWithMilliSeconds(100, 100));
    TEST_ASSERT_EQUAL(100, systemStatus.getlastStatusSaveTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(100, systemStatus.getStatus().lastStatusSaveTimeStamp.getUnixTime());
}

void test_wifiStatus() {
    TEST_ASSERT_EQUAL(WL_DISCONNECTED, systemStatus.getStatus().wifiStatus);
    TEST_ASSERT_FALSE(systemStatus.isWifiConnected());
    systemStatus.setWifiStatus(WL_CONNECTED);
    TEST_ASSERT_EQUAL(WL_CONNECTED, systemStatus.getStatus().wifiStatus);
    TEST_ASSERT_TRUE(systemStatus.isWifiConnected());
}

void test_timeStatus() {
    TEST_ASSERT_FALSE(systemStatus.isTimeSet());
    systemStatus.setClockStatus(true);
    TEST_ASSERT_TRUE(systemStatus.isTimeSet());
    TEST_ASSERT_TRUE(systemStatus.getStatus().isTimeSet);
}

void test_toJsonDocument() {
    Blackout expectedLastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 100), UnixTimeWithMilliSeconds(200, 200));
    Blackout expectedShortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 300),
                                                 UnixTimeWithMilliSeconds(400, 400));
    Blackout expectedLongestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 500), UnixTimeWithMilliSeconds(600, 600));

    SystemStatus::Data status;
    status.wifiStatus = WL_CONNECTED;
    status.isPersistentStorageFailed = true;
    status.isTimeSet = true;
    status.isAlarmActive = true;
    status.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(50, 100);
    status.lastStatusSaveTimeStamp = UnixTimeWithMilliSeconds(500, 200);
    status.lastRebootTimeStamp = UnixTimeWithMilliSeconds(600, 300);
    status.lastResetTimeStamp = UnixTimeWithMilliSeconds(700, 400);
    status.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(800, 500);
    status.rebootCount = 5;
    status.blackoutCount = 3;
    status.lastBlackout = expectedLastBlackout;
    status.shortestBlackout = expectedShortestBlackout;
    status.longestBlackout = expectedLongestBlackout;

    systemStatus = SystemStatus(status);

    JsonDocument doc = systemStatus.toJsonDocument();

    TEST_ASSERT_EQUAL(WL_CONNECTED, doc[SYSTEM_STATUS_FIELD_WIFI_STATUS]);
    TEST_ASSERT_EQUAL(true, doc[SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED]);
    TEST_ASSERT_EQUAL(true, doc[SYSTEM_STATUS_FIELD_IS_TIME_SET]);
    TEST_ASSERT_EQUAL(true, doc[SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE]);
    TEST_ASSERT_EQUAL(50, doc[SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(500, doc[SYSTEM_STATUS_FIELD_LAST_STATUS_SAVE_TIMESTAMP][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(600, doc[SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(700, doc[SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(800, doc[SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(5, doc[SYSTEM_STATUS_FIELD_REBOOT_COUNT]);
    TEST_ASSERT_EQUAL(3, doc[SYSTEM_STATUS_FIELD_BLACKOUT_COUNT]);

    TEST_ASSERT_TRUE(expectedLastBlackout.equals(Blackout(doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT].as<JsonObject>())));
    TEST_ASSERT_TRUE(
            expectedShortestBlackout.equals(Blackout(doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT].as<JsonObject>())));
    TEST_ASSERT_TRUE(
            expectedLongestBlackout.equals(Blackout(doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT].as<JsonObject>())));
}

void test_toHumanReadableJsonDocument() {
    Blackout expectedLastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 100), UnixTimeWithMilliSeconds(200, 200));
    Blackout expectedShortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 300),
                                                 UnixTimeWithMilliSeconds(400, 400));
    Blackout expectedLongestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 500), UnixTimeWithMilliSeconds(600, 600));

    SystemStatus::Data status;
    status.wifiStatus = WL_CONNECTED;
    status.isPersistentStorageFailed = true;
    status.isTimeSet = true;
    status.isAlarmActive = true;
    status.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(50, 100);
    status.lastStatusSaveTimeStamp = UnixTimeWithMilliSeconds(500, 200);
    status.lastRebootTimeStamp = UnixTimeWithMilliSeconds(600, 300);
    status.lastResetTimeStamp = UnixTimeWithMilliSeconds(700, 400);
    status.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(800, 500);
    status.rebootCount = 5;
    status.blackoutCount = 3;
    status.lastBlackout = expectedLastBlackout;
    status.shortestBlackout = expectedShortestBlackout;
    status.longestBlackout = expectedLongestBlackout;

    systemStatus = SystemStatus(status);

    JsonDocument doc = systemStatus.toHumanReadableJsonDocument();

    TEST_ASSERT_EQUAL(true, doc[SYSTEM_STATUS_FIELD_IS_TIME_SET]);
    TEST_ASSERT_EQUAL(true, doc[SYSTEM_STATUS_FIELD_IS_ALARM_ACTIVE]);
    TEST_ASSERT_EQUAL(true, doc[SYSTEM_STATUS_FIELD_IS_PERSISTENT_STORAGE_FAILED]);
    TEST_ASSERT_EQUAL_STRING("1970-01-01 00:00:50.100", doc[SYSTEM_STATUS_FIELD_LAST_PERSISTENT_STORAGE_FAILED_TIMESTAMP]);
    TEST_ASSERT_EQUAL_STRING("1970-01-01 00:08:20.200", doc[SYSTEM_STATUS_FIELD_LAST_STATUS_SAVE_TIMESTAMP]);
    TEST_ASSERT_EQUAL_STRING("1970-01-01 00:10:00.300", doc[SYSTEM_STATUS_FIELD_LAST_REBOOT_TIMESTAMP]);
    TEST_ASSERT_EQUAL_STRING("1970-01-01 00:11:40.400", doc[SYSTEM_STATUS_FIELD_LAST_RESET_TIMESTAMP]);
    TEST_ASSERT_EQUAL_STRING("1970-01-01 00:13:20.500", doc[SYSTEM_STATUS_FIELD_LAST_ALARM_CLEARED_TIMESTAMP]);
    TEST_ASSERT_EQUAL(5, doc[SYSTEM_STATUS_FIELD_REBOOT_COUNT]);
    TEST_ASSERT_EQUAL(3, doc[SYSTEM_STATUS_FIELD_BLACKOUT_COUNT]);

    TEST_ASSERT_EQUAL_STRING(expectedLastBlackout.getStart().getFormattedTime().c_str(), doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT][BLACKOUT_FIELD_START]);
    TEST_ASSERT_EQUAL_STRING(expectedLastBlackout.getEnd().getFormattedTime().c_str(), doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT][BLACKOUT_FIELD_END]);
    TEST_ASSERT_EQUAL_STRING(expectedLastBlackout.getDuration().getFormattedDuration().c_str(), doc[SYSTEM_STATUS_FIELD_LAST_BLACKOUT][BLACKOUT_FIELD_DURATION]);

    TEST_ASSERT_EQUAL_STRING(expectedShortestBlackout.getStart().getFormattedTime().c_str(), doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT][BLACKOUT_FIELD_START]);
    TEST_ASSERT_EQUAL_STRING(expectedShortestBlackout.getEnd().getFormattedTime().c_str(), doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT][BLACKOUT_FIELD_END]);
    TEST_ASSERT_EQUAL_STRING(expectedShortestBlackout.getDuration().getFormattedDuration().c_str(), doc[SYSTEM_STATUS_FIELD_SHORTEST_BLACKOUT][BLACKOUT_FIELD_DURATION]);

    TEST_ASSERT_EQUAL_STRING(expectedLongestBlackout.getStart().getFormattedTime().c_str(), doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT][BLACKOUT_FIELD_START]);
    TEST_ASSERT_EQUAL_STRING(expectedLongestBlackout.getEnd().getFormattedTime().c_str(), doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT][BLACKOUT_FIELD_END]);
    TEST_ASSERT_EQUAL_STRING(expectedLongestBlackout.getDuration().getFormattedDuration().c_str(), doc[SYSTEM_STATUS_FIELD_LONGEST_BLACKOUT][BLACKOUT_FIELD_DURATION]);
}

void test_equals_withEqual() {
    SystemStatus::Data status;
    status.wifiStatus = WL_CONNECTED;
    status.isPersistentStorageFailed = true;
    status.isTimeSet = true;
    status.isAlarmActive = true;
    status.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(50, 100);
    status.lastStatusSaveTimeStamp = UnixTimeWithMilliSeconds(150, 200);
    status.lastRebootTimeStamp = UnixTimeWithMilliSeconds(250, 300);
    status.lastResetTimeStamp = UnixTimeWithMilliSeconds(350, 400);
    status.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(450, 500);
    status.rebootCount = 5;
    status.blackoutCount = 3;
    status.lastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 150), UnixTimeWithMilliSeconds(200, 250));
    status.shortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 350), UnixTimeWithMilliSeconds(400, 450));
    status.longestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 550), UnixTimeWithMilliSeconds(600, 650));

    systemStatus = SystemStatus(status);

    TEST_ASSERT_TRUE(systemStatus.equals(SystemStatus(status)));
}

void test_equals_withDifferent() {
    SystemStatus::Data status1;
    status1.wifiStatus = WL_CONNECTED;
    status1.isPersistentStorageFailed = true;
    status1.isTimeSet = true;
    status1.isAlarmActive = true;
    status1.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(50, 100);
    status1.lastStatusSaveTimeStamp = UnixTimeWithMilliSeconds(150, 200);
    status1.lastRebootTimeStamp = UnixTimeWithMilliSeconds(250, 300);
    status1.lastResetTimeStamp = UnixTimeWithMilliSeconds(350, 400);
    status1.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(450, 500);
    status1.rebootCount = 5;
    status1.blackoutCount = 3;
    status1.lastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 150), UnixTimeWithMilliSeconds(200, 250));
    status1.shortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 350), UnixTimeWithMilliSeconds(400, 450));
    status1.longestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 550), UnixTimeWithMilliSeconds(600, 650));

    SystemStatus::Data status2;
    status2.wifiStatus = WL_CONNECTED;
    status2.isPersistentStorageFailed = true;
    status2.isTimeSet = true;
    status2.isAlarmActive = false;
    status2.lastPersistentStorageFailedTimeStamp = UnixTimeWithMilliSeconds(50, 100);
    status2.lastStatusSaveTimeStamp = UnixTimeWithMilliSeconds(150, 200);
    status2.lastRebootTimeStamp = UnixTimeWithMilliSeconds(250, 300);
    status2.lastResetTimeStamp = UnixTimeWithMilliSeconds(350, 400);
    status2.lastAlarmClearedTimeStamp = UnixTimeWithMilliSeconds(450, 500);
    status2.rebootCount = 5;
    status2.blackoutCount = 3;
    status2.lastBlackout = Blackout(UnixTimeWithMilliSeconds(100, 150), UnixTimeWithMilliSeconds(200, 250));
    status2.shortestBlackout = Blackout(UnixTimeWithMilliSeconds(300, 350), UnixTimeWithMilliSeconds(400, 450));
    status2.longestBlackout = Blackout(UnixTimeWithMilliSeconds(500, 550), UnixTimeWithMilliSeconds(600, 650));

    TEST_ASSERT_FALSE(systemStatus.equals(SystemStatus(status2)));
}


bool statusEquals(SystemStatus::Data status1, SystemStatus::Data status2) {
    return status1.wifiStatus == status2.wifiStatus
           && status1.isPersistentStorageFailed == status2.isPersistentStorageFailed
           && status1.isTimeSet == status2.isTimeSet
           && status1.isAlarmActive == status2.isAlarmActive
           && status1.lastPersistentStorageFailedTimeStamp.equals(status2.lastPersistentStorageFailedTimeStamp)
           && status1.lastStatusSaveTimeStamp.equals(status2.lastStatusSaveTimeStamp)
           && status1.lastRebootTimeStamp.equals(status2.lastRebootTimeStamp)
           && status1.lastResetTimeStamp.equals(status2.lastResetTimeStamp)
           && status1.lastAlarmClearedTimeStamp.equals(status2.lastAlarmClearedTimeStamp)
           && status1.rebootCount == status2.rebootCount
           && status1.blackoutCount == status2.blackoutCount
           && status1.lastBlackout.equals(status2.lastBlackout)
           && status1.shortestBlackout.equals(status2.shortestBlackout)
           && status1.longestBlackout.equals(status2.longestBlackout);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_constructor_default);
    RUN_TEST(test_constructor_withStatusStruct);
    RUN_TEST(test_constructor_withJsonDocument);

    RUN_TEST(test_getStatus_initial);

    RUN_TEST(test_rebootDetected);
    RUN_TEST(test_persistentStorageFailed);

    RUN_TEST(test_getLastBlackout_initial);
    RUN_TEST(test_setPowerStatus_withPowerGoingOffline);
    RUN_TEST(test_setPowerStatus_withPowerGoingOnline);
    RUN_TEST(test_setPowerStatus_withMultipleBlackouts);
    RUN_TEST(test_setPowerStatus_withPowerGoingOffline_duringOngoingBlackout);
    RUN_TEST(test_setPowerStatus_withPowerGoingOnline_withoutActiveBlackout);

    RUN_TEST(test_clearAlarm);
    RUN_TEST(test_reset_withNoActiveBlackout);
    RUN_TEST(test_reset_withActiveBlackout);
    RUN_TEST(test_lastStatusSaveTimestamp);
    RUN_TEST(test_wifiStatus);
    RUN_TEST(test_timeStatus);
    RUN_TEST(test_toJsonDocument);
    RUN_TEST(test_toHumanReadableJsonDocument);

    RUN_TEST(test_equals_withEqual);
    RUN_TEST(test_equals_withDifferent);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}

#pragma clang diagnostic pop