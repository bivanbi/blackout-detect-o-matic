#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_CommandLineInterface.h"

String systemStatusFilePath = TEST_DIRECTORY "/test-status.json";

void setUp(void) {
    configuration.setSystemStatusFilePath(systemStatusFilePath);

    if (!persistentStorage.isMounted()) {
        persistentStorage.mount();
    }

    if (!persistentStorage.exists(TEST_DIRECTORY)) {
        persistentStorage.createDirectory(TEST_DIRECTORY);
    }

    if (persistentStorage.exists(systemStatusFilePath)) {
        persistentStorage.removeFile(systemStatusFilePath);
    }
}

void tearDown(void) {
}

void test_executeCommand_withUnknownCommand() {
    TEST_ASSERT_EQUAL_STRING("unknown command", commandLineInterface.executeCommand("unknown").c_str());
}

void test_executeCommand_ping() {
    TEST_ASSERT_EQUAL_STRING("pong", commandLineInterface.executeCommand("ping").c_str());
}

void test_executeCommand_clock() {
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, UnixTimeWithMilliSeconds(123, 000));
    String expected = "Clock source: " + rtcAdapter.clockSourceToString(RTCAdapter::NTP) + ", time: " +
                      UnixTimeWithMilliSeconds(123, 456).getFormattedTime();

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), commandLineInterface.executeCommand("clock").c_str());
}

void test_executeCommand_status() {
    String expected;
    serializeJsonPretty(systemStatus.toHumanReadableJsonDocument(), expected);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), commandLineInterface.executeCommand("status").c_str());
}

void test_executeCommand_clearAlarm() {
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(123, 456));

    TEST_ASSERT_EQUAL_STRING("Alarm cleared", commandLineInterface.executeCommand("clearAlarm").c_str());
    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
}

void test_executeCommand_resetStatus() {
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(123, 456));

    TEST_ASSERT_EQUAL_STRING("Status reset", commandLineInterface.executeCommand("resetStatus").c_str());
    TEST_ASSERT_EQUAL(0, systemStatus.getRebootCount());
}

void test_uptime() {
    unsigned long expectedMinimumUptimeSeconds = millis() / 1000;
    unsigned long expectedMinimumUptimeMillis = millis() % 1000;

    String expectedUptime = rtcAdapter.getTime().getFormattedTime() + " up "
                            + Duration(1, expectedMinimumUptimeSeconds, expectedMinimumUptimeMillis).getFormattedDuration();
    String actualUptime = commandLineInterface.executeCommand("uptime");

    TEST_ASSERT_EQUAL_STRING(expectedUptime.c_str(), actualUptime.c_str());
}

void test_help() {
    String expected = "reboot <delay> - reboot the system, optional delay in seconds, default: " + String(CLI_DEFAULT_REBOOT_DELAY) + " s\n"
                      "ping - echo request\n"
                      "clock - get the current time\n"
                      "config - configuration commands, issue 'config help' for more info\n"
                      "uptime - get the uptime\n"
                      "status - get the status of the system\n"
                      "clearAlarm - clear the alarm\n"
                      "resetStatus - reset reboot / blackout counters and clear alarm\n"
                      "saveStatus - save status to persistent storage (SD card)\n";

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), commandLineInterface.executeCommand("help").c_str());
}

void test_saveSystemStatus() {
    String response = commandLineInterface.executeCommand("saveStatus");

    TEST_ASSERT_EQUAL_STRING("Status saved", response.c_str());
    TEST_ASSERT_TRUE(persistentStorage.exists(systemStatusFilePath));
    TEST_ASSERT_EQUAL_STRING(persistentStorage.readFile(systemStatusFilePath).c_str(), systemStatus.toJsonDocument().as<String>().c_str());
}

void test_splitCommandAndArguments_withEmptyString() {
    CommandLineInterface::CommandAndArguments actual = commandLineInterface.splitCommandAndArguments("");

    TEST_ASSERT_EQUAL_STRING("", actual.command.c_str());
    TEST_ASSERT_EQUAL_STRING("", actual.arguments.c_str());
}

void test_splitCommandAndArguments_withNoArguments() {
    CommandLineInterface::CommandAndArguments actual = commandLineInterface.splitCommandAndArguments("config");

    TEST_ASSERT_EQUAL_STRING("config", actual.command.c_str());
    TEST_ASSERT_EQUAL_STRING("", actual.arguments.c_str());
}

void test_splitCommandAndArguments_withMultipleArguments() {
    CommandLineInterface::CommandAndArguments actual = commandLineInterface.splitCommandAndArguments("config set this=that");

    TEST_ASSERT_EQUAL_STRING("config", actual.command.c_str());
    TEST_ASSERT_EQUAL_STRING("set this=that", actual.arguments.c_str());
}

void test_config_get() {
    String actual = commandLineInterface.executeCommand("config get");

    TEST_ASSERT_GREATER_THAN(0, actual.indexOf(CONFIGURATION_FIELD_WIFI_SSID));
}

void test_config_get_singleField() {
    configuration.setWifiSSID("TestSSID");

    String actual = commandLineInterface.executeCommand("config get wifiSSID");

    TEST_ASSERT_GREATER_THAN(-1, actual.indexOf("TestSSID"));
    TEST_ASSERT_EQUAL_STRING("TestSSID", configuration.getWifiSSID().c_str());
}

void test_config_set() {
    String actual = commandLineInterface.executeCommand("config set wifiSSID=TestSSID");

    TEST_ASSERT_GREATER_THAN(-1, actual.indexOf("updated"));
    TEST_ASSERT_EQUAL_STRING("TestSSID", configuration.getWifiSSID().c_str());
}

void test_config_save() {
    configuration.setWifiSSID("TestSSID");

    String actual = commandLineInterface.executeCommand("config save");

    TEST_ASSERT_GREATER_THAN(-1, actual.indexOf("saved"));
    TEST_ASSERT_EQUAL_STRING(persistentStorage.readFile(CONFIGURATION_FILE_PATH).c_str(), configuration.toJsonDocument().as<String>().c_str());
}

int runUnitTests() {
    UNITY_BEGIN();

    RUN_TEST(test_executeCommand_withUnknownCommand);
    RUN_TEST(test_executeCommand_ping);
    RUN_TEST(test_executeCommand_clock);
    RUN_TEST(test_executeCommand_status);
    RUN_TEST(test_executeCommand_clearAlarm);
    RUN_TEST(test_executeCommand_resetStatus);
    RUN_TEST(test_uptime);
    RUN_TEST(test_help);
    RUN_TEST(test_saveSystemStatus);

    RUN_TEST(test_splitCommandAndArguments_withEmptyString);
    RUN_TEST(test_splitCommandAndArguments_withNoArguments);
    RUN_TEST(test_splitCommandAndArguments_withMultipleArguments);
    RUN_TEST(test_config_get);
    RUN_TEST(test_config_get_singleField);
    RUN_TEST(test_config_set);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnitTests();
}

void loop() {
}

#pragma clang diagnostic pop