#include "test_Configuration.h"

void setUp(void) {
    configuration = Configuration();
}

void tearDown(void) {
}

void test_constructor_default() {
    TEST_ASSERT_EQUAL(HEARTBEAT_FILE_LOG_INTERVAL, configuration.getHeartbeatFileLogInterval());
    TEST_ASSERT_EQUAL(HEARTBEAT_SERIAL_LOG_INTERVAL, configuration.getHeartbeatSerialLogInterval());
    TEST_ASSERT_EQUAL_STRING(LOG_DIRECTORY, configuration.getLogDirectory().c_str());
    TEST_ASSERT_EQUAL_STRING(LOG_FILE_NAME, configuration.getLogFileName().c_str());
    TEST_ASSERT_EQUAL_STRING(NTP_SERVER, configuration.getNtpServer().c_str());
    TEST_ASSERT_EQUAL(NTP_OFFSET, configuration.getNtpOffset());
    TEST_ASSERT_EQUAL(NTP_UPDATE_INTERVAL, configuration.getNtpUpdateInterval());
    TEST_ASSERT_EQUAL_STRING(SYSTEM_STATUS_FILE_PATH, configuration.getSystemStatusFilePath().c_str());
    TEST_ASSERT_EQUAL(TELNET_SERVER_PORT, configuration.getTelnetServerPort());
    TEST_ASSERT_EQUAL_STRING(WIFI_SECRET, configuration.getWifiSecret().c_str());
    TEST_ASSERT_EQUAL_STRING(WIFI_SSID, configuration.getWifiSSID().c_str());
}

void test_constructor_withJsonDocument() {
    JsonDocument doc;
    doc[CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL] = 654245;
    doc[CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL] = 987;
    doc[CONFIGURATION_FIELD_LOG_FILE_NAME] = "dummy-log-file-name.log";
    doc[CONFIGURATION_FIELD_LOG_DIRECTORY] = "/dummy-log-directory";
    doc[CONFIGURATION_FIELD_NTP_OFFSET] = 123;
    doc[CONFIGURATION_FIELD_NTP_SERVER] = "test.europe.pool.ntp.org";
    doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL] = 456;
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH] = "/dummy-system-status-file-path";
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL] = 654245;
    doc[CONFIGURATION_FIELD_TELNET_SERVER_PORT] = 789;
    doc[CONFIGURATION_FIELD_WIFI_SECRET] = "TestDummyWifiPassword";
    doc[CONFIGURATION_FIELD_WIFI_SSID] = "TestDummyWifiSSID";

    configuration = Configuration(doc.as<JsonObject>());

    TEST_ASSERT_EQUAL(654245, configuration.getHeartbeatFileLogInterval());
    TEST_ASSERT_EQUAL(987, configuration.getHeartbeatSerialLogInterval());
    TEST_ASSERT_EQUAL_STRING("dummy-log-file-name.log", configuration.getLogFileName().c_str());
    TEST_ASSERT_EQUAL_STRING("/dummy-log-directory", configuration.getLogDirectory().c_str());
    TEST_ASSERT_EQUAL(123, configuration.getNtpOffset());
    TEST_ASSERT_EQUAL_STRING("test.europe.pool.ntp.org", configuration.getNtpServer().c_str());
    TEST_ASSERT_EQUAL(456, configuration.getNtpUpdateInterval());
    TEST_ASSERT_EQUAL_STRING("/dummy-system-status-file-path", configuration.getSystemStatusFilePath().c_str());
    TEST_ASSERT_EQUAL(654245, configuration.getSystemStatusSaveInterval());
    TEST_ASSERT_EQUAL(789, configuration.getTelnetServerPort());
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiSSID", configuration.getWifiSSID().c_str());
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiPassword", configuration.getWifiSecret().c_str());
}

void test_toJsonDocument() {
    JsonDocument doc = configuration.toJsonDocument();

    TEST_ASSERT_EQUAL(HEARTBEAT_FILE_LOG_INTERVAL, doc[CONFIGURATION_FIELD_HEARTBEAT_FILE_LOG_INTERVAL]);
    TEST_ASSERT_EQUAL(HEARTBEAT_SERIAL_LOG_INTERVAL, doc[CONFIGURATION_FIELD_HEARTBEAT_SERIAL_LOG_INTERVAL]);
    TEST_ASSERT_EQUAL_STRING(LOG_FILE_NAME, doc[CONFIGURATION_FIELD_LOG_FILE_NAME].as<String>().c_str());
    TEST_ASSERT_EQUAL_STRING(LOG_DIRECTORY, doc[CONFIGURATION_FIELD_LOG_DIRECTORY].as<String>().c_str());
    TEST_ASSERT_EQUAL(NTP_OFFSET, doc[CONFIGURATION_FIELD_NTP_OFFSET]);
    TEST_ASSERT_EQUAL_STRING(NTP_SERVER, doc[CONFIGURATION_FIELD_NTP_SERVER].as<String>().c_str());
    TEST_ASSERT_EQUAL(NTP_UPDATE_INTERVAL, doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL]);
    TEST_ASSERT_EQUAL_STRING(SYSTEM_STATUS_FILE_PATH, doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH].as<String>().c_str());
    TEST_ASSERT_EQUAL(SYSTEM_STATUS_SAVE_INTERVAL, doc[CONFIGURATION_FIELD_SYSTEM_STATUS_SAVE_INTERVAL]);
    TEST_ASSERT_EQUAL(TELNET_SERVER_PORT, doc[CONFIGURATION_FIELD_TELNET_SERVER_PORT]);
    TEST_ASSERT_EQUAL_STRING(WIFI_SECRET, doc[CONFIGURATION_FIELD_WIFI_SECRET].as<String>().c_str());
    TEST_ASSERT_EQUAL_STRING(WIFI_SSID, doc[CONFIGURATION_FIELD_WIFI_SSID].as<String>().c_str());
}

void test_setHeartbeatFileLogInterval() {
    configuration.setHeartbeatFileLogInterval(654245);
    TEST_ASSERT_EQUAL(654245, configuration.getHeartbeatFileLogInterval());
}

void test_setHeartbeatSerialLogInterval() {
    configuration.setHeartbeatSerialLogInterval(987);
    TEST_ASSERT_EQUAL(987, configuration.getHeartbeatSerialLogInterval());
}

void test_setLogFileName() {
    configuration.setLogFileName("dummy-log-file-name.log");
    TEST_ASSERT_EQUAL_STRING("dummy-log-file-name.log", configuration.getLogFileName().c_str());
}

void test_setLogDirectory() {
    configuration.setLogDirectory("/dummy-log-directory");
    TEST_ASSERT_EQUAL_STRING("/dummy-log-directory", configuration.getLogDirectory().c_str());
}

void test_setNtpOffset() {
    configuration.setNtpOffset(123);
    TEST_ASSERT_EQUAL(123, configuration.getNtpOffset());
}

void test_setNtpServer() {
    configuration.setNtpServer("test.europe.pool.ntp.org");
    TEST_ASSERT_EQUAL_STRING("test.europe.pool.ntp.org", configuration.getNtpServer().c_str());
}

void test_setNtpUpdateInterval() {
    configuration.setNtpUpdateInterval(456);
    TEST_ASSERT_EQUAL(456, configuration.getNtpUpdateInterval());
}

void test_setSystemStatusFilePath() {
    configuration.setSystemStatusFilePath("/dummy-system-status-file-path");
    TEST_ASSERT_EQUAL_STRING("/dummy-system-status-file-path", configuration.getSystemStatusFilePath().c_str());
}

void test_setSystemStatusSaveInterval() {
    configuration.setSystemStatusSaveInterval(1234567);
    TEST_ASSERT_EQUAL(1234567, configuration.getSystemStatusSaveInterval());
}

void test_setTelnetServerPort() {
    configuration.setTelnetServerPort(789);
    TEST_ASSERT_EQUAL(789, configuration.getTelnetServerPort());
}

void test_setWifiSecret() {
    configuration.setWifiSecret("TestDummyWifiPassword");
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiPassword", configuration.getWifiSecret().c_str());
}

void test_setWifiSSID() {
    configuration.setWifiSSID("TestDummyWifiSSID");
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiSSID", configuration.getWifiSSID().c_str());
}

void test_set_withInvalidKey() {
    Configuration::SetResult result = configuration.set("invalid-key", "dummy-value");
    TEST_ASSERT_EQUAL(Configuration::ResultCode::INVALID_KEY, result.code);
}

void test_set_withValidKey() {
    Configuration::SetResult result = configuration.set(CONFIGURATION_FIELD_WIFI_SSID, "TestDummyWifiSSID");
    TEST_ASSERT_EQUAL(Configuration::ResultCode::OK, result.code);
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiSSID", configuration.getWifiSSID().c_str());
}

void test_get_withInvalidKey() {
    Configuration::GetResult result = configuration.get("invalid-key");
    TEST_ASSERT_EQUAL(Configuration::ResultCode::INVALID_KEY, result.code);
}

void test_get_withValidKey() {
    Configuration::SetResult result = configuration.set(CONFIGURATION_FIELD_WIFI_SSID, "TestDummyWifiSSID");
    TEST_ASSERT_EQUAL(Configuration::ResultCode::OK, result.code);
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiSSID", configuration.getWifiSSID().c_str());
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_constructor_default);
    RUN_TEST(test_constructor_withJsonDocument);
    RUN_TEST(test_toJsonDocument);

    RUN_TEST(test_setHeartbeatFileLogInterval);
    RUN_TEST(test_setHeartbeatSerialLogInterval);
    RUN_TEST(test_setLogFileName);
    RUN_TEST(test_setLogDirectory);
    RUN_TEST(test_setNtpOffset);
    RUN_TEST(test_setNtpServer);
    RUN_TEST(test_setNtpUpdateInterval);
    RUN_TEST(test_setSystemStatusFilePath);
    RUN_TEST(test_setSystemStatusSaveInterval);
    RUN_TEST(test_setTelnetServerPort);
    RUN_TEST(test_setWifiSecret);
    RUN_TEST(test_setWifiSSID);

    RUN_TEST(test_set_withInvalidKey);
    RUN_TEST(test_set_withValidKey);

    RUN_TEST(test_get_withInvalidKey);
    RUN_TEST(test_get_withValidKey);

    UNITY_END();
    return 0;
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}
