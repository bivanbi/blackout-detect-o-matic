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

void test_executeCommand_withUnknownCommand() {
    TEST_ASSERT_EQUAL_STRING("unknown command", commandLineInterface.executeCommand("unknown").c_str());
}

void test_alarm_clear() {
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(123, 456));

    TEST_ASSERT_EQUAL_STRING("Alarm cleared", commandLineInterface.executeCommand("alarm clear").c_str());
    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
}

void test_date() {
    rtcAdapter.pause();
    rtcAdapter.setTime(RTCAdapter::NTP, UnixTimeWithMilliSeconds(123, 458));
    String expected = "Clock source: " + rtcAdapter.clockSourceToString(RTCAdapter::NTP) + ", time: " +
                      UnixTimeWithMilliSeconds(123, 458).getFormattedTime();

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), commandLineInterface.executeCommand("date").c_str());
}

void test_help() {
    String actualHelp = commandLineInterface.executeCommand("help");
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_ALARM));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_CONFIG));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_DATE));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_HELP));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_MEMINFO));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_REBOOT));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_SDCARD));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_STATUS));
    TEST_ASSERT_GREATER_OR_EQUAL(0, actualHelp.indexOf(CLI_COMMAND_UPTIME));
}

void test_meminfo() {
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("meminfo").indexOf("Total heap:"));
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("meminfo").indexOf("Free heap:"));
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("meminfo").indexOf("Total PSRAM:"));
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("meminfo").indexOf("Free PSRAM:"));
}

void test_uptime() {
    rtcAdapter.pause();
    rtcAdapter.setTime(RTCAdapter::NTP, UnixTimeWithMilliSeconds(123456, 456));
    UptimeAdapter::pause();
    UptimeAdapter::set(123456);

    TEST_ASSERT_EQUAL_STRING("1970-01-02 10:17:36 up 0 days 00:02:03.456", commandLineInterface.executeCommand("uptime").c_str());
}

void test_sdcard_usage() {
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("sdcard usage").indexOf("Total space:"));
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("sdcard usage").indexOf("Used space:"));
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("sdcard usage").indexOf("Free space:"));
}

void test_sdcard_list() {
    TEST_ASSERT_GREATER_OR_EQUAL(0, commandLineInterface.executeCommand("sdcard list").indexOf("d "));
}

void test_sdcard_cat() {
    String path = TEST_DIRECTORY "/test-file.txt";
    String content = "Test file content";
    persistentStorage.writeFile(path, content);

    TEST_ASSERT_EQUAL_STRING(content.c_str(), commandLineInterface.executeCommand("sdcard cat " + path).c_str());
}

void test_sdcard_remove() {
    String path = TEST_DIRECTORY "/test-file.txt";
    persistentStorage.writeFile(path, "Test file content");

    TEST_ASSERT_TRUE(persistentStorage.exists(path));
    TEST_ASSERT_EQUAL_STRING("File removed", commandLineInterface.executeCommand("sdcard remove " + path).c_str());
    TEST_ASSERT_FALSE(persistentStorage.exists(path));
}

void test_status_get() {
    String expected;
    serializeJsonPretty(systemStatus.toHumanReadableJsonDocument(), expected);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), commandLineInterface.executeCommand("status get").c_str());
}

void test_status_reset() {
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(123, 456));

    TEST_ASSERT_EQUAL_STRING("Status reset", commandLineInterface.executeCommand("status reset").c_str());
    TEST_ASSERT_EQUAL(0, systemStatus.getRebootCount());
}

void test_status_save() {
    String response = commandLineInterface.executeCommand("status save");

    TEST_ASSERT_EQUAL_STRING("Status saved", response.c_str());
    TEST_ASSERT_TRUE(persistentStorage.exists(systemStatusFilePath));
    TEST_ASSERT_EQUAL_STRING(persistentStorage.readFile(systemStatusFilePath).c_str(), systemStatus.toJsonDocument().as<String>().c_str());
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

    RUN_TEST(test_splitCommandAndArguments_withEmptyString);
    RUN_TEST(test_splitCommandAndArguments_withNoArguments);
    RUN_TEST(test_splitCommandAndArguments_withMultipleArguments);

    RUN_TEST(test_executeCommand_withUnknownCommand);
    RUN_TEST(test_alarm_clear);
    RUN_TEST(test_config_get);
    RUN_TEST(test_config_get_singleField);
    RUN_TEST(test_config_set);
    RUN_TEST(test_date);
    RUN_TEST(test_help);
    RUN_TEST(test_meminfo);
    RUN_TEST(test_sdcard_usage);
    RUN_TEST(test_sdcard_list);
    RUN_TEST(test_sdcard_cat);
    RUN_TEST(test_sdcard_remove);
    RUN_TEST(test_status_get);
    RUN_TEST(test_status_reset);
    RUN_TEST(test_status_save);
    RUN_TEST(test_uptime);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnitTests();
}

void loop() {
}

#pragma clang diagnostic pop