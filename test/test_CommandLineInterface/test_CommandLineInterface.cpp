#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_CommandLineInterface.h"

void setUp(void) {
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

    String actualDuration = commandLineInterface.executeCommand("uptime");
    String expectedDuration = Duration(1, expectedMinimumUptimeSeconds, expectedMinimumUptimeMillis).getFormattedDuration();

    TEST_ASSERT_EQUAL_STRING(expectedDuration.c_str(), actualDuration.c_str());
}

void test_help() {
    String expected = "ping - echo request\n"
                      "clock - get the current time\n"
                      "uptime - get the uptime\n"
                      "status - get the status of the system\n"
                      "clearAlarm - clear the alarm\n"
                      "resetStatus - reset reboot / blackout counters and clear alarm\n"
                      "saveStatus - save status to persistent storage (SD card)\n";

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), commandLineInterface.executeCommand("help").c_str());
}

void test_saveSystemStatus() {
    String testFilePath = "/testCommandLineInterfaceSystemStatus.json";
    configuration.setSystemStatusFilePath(testFilePath);

    if (!persistentStorage.isMounted()) {
        persistentStorage.mount();
    }

    if (persistentStorage.exists(testFilePath)) {
        persistentStorage.removeFile(testFilePath);
    }

    String response = commandLineInterface.executeCommand("saveStatus");

    TEST_ASSERT_EQUAL_STRING("Status saved", response.c_str());
    TEST_ASSERT_TRUE(persistentStorage.exists(testFilePath));
    TEST_ASSERT_EQUAL_STRING(persistentStorage.readFile(testFilePath).c_str(), systemStatus.toJsonDocument().as<String>().c_str());
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

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnitTests();
}

void loop() {
}

#pragma clang diagnostic pop