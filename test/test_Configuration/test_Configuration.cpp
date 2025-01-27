#include "test_Configuration.h"

void setUp(void) {
    configuration = Configuration();
}

void tearDown(void) {
}

void test_constructor_default() {
    TEST_ASSERT_EQUAL_STRING(LOG_DIRECTORY, configuration.getLogDirectory().c_str());
    TEST_ASSERT_EQUAL_STRING(SYSTEM_STATUS_FILE_PATH, configuration.getSystemStatusFilePath().c_str());
    TEST_ASSERT_EQUAL_STRING(WIFI_SSID, configuration.getWifiSSID().c_str());
    TEST_ASSERT_EQUAL_STRING(WIFI_SECRET, configuration.getWifiSecret().c_str());
    TEST_ASSERT_EQUAL_STRING(NTP_SERVER, configuration.getNtpServer().c_str());
    TEST_ASSERT_EQUAL(NTP_OFFSET, configuration.getNtpOffset());
    TEST_ASSERT_EQUAL(NTP_UPDATE_INTERVAL, configuration.getNtpUpdateInterval());
}

void test_constructor_withJsonDocument() {
    JsonDocument doc;
    doc[CONFIGURATION_FIELD_LOG_DIRECTORY] = "/dummy-log-directory";
    doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH] = "/dummy-system-status-file-path";
    doc[CONFIGURATION_FIELD_WIFI_SSID] = "TestDummyWifiSSID";
    doc[CONFIGURATION_FIELD_WIFI_SECRET] = "TestDummyWifiPassword";
    doc[CONFIGURATION_FIELD_NTP_SERVER] = "test.europe.pool.ntp.org";
    doc[CONFIGURATION_FIELD_NTP_OFFSET] = 123;
    doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL] = 456;

    configuration = Configuration(doc.as<JsonObject>());

    TEST_ASSERT_EQUAL_STRING("/dummy-log-directory", configuration.getLogDirectory().c_str());
    TEST_ASSERT_EQUAL_STRING("/dummy-system-status-file-path", configuration.getSystemStatusFilePath().c_str());
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiSSID", configuration.getWifiSSID().c_str());
    TEST_ASSERT_EQUAL_STRING("TestDummyWifiPassword", configuration.getWifiSecret().c_str());
    TEST_ASSERT_EQUAL_STRING("test.europe.pool.ntp.org", configuration.getNtpServer().c_str());
    TEST_ASSERT_EQUAL(123, configuration.getNtpOffset());
    TEST_ASSERT_EQUAL(456, configuration.getNtpUpdateInterval());
}

void test_toJsonDocument() {
    JsonDocument doc = configuration.toJsonDocument();

    TEST_ASSERT_EQUAL_STRING(LOG_DIRECTORY, doc[CONFIGURATION_FIELD_LOG_DIRECTORY].as<String>().c_str());
    TEST_ASSERT_EQUAL_STRING(SYSTEM_STATUS_FILE_PATH, doc[CONFIGURATION_FIELD_SYSTEM_STATUS_FILE_PATH].as<String>().c_str());
    TEST_ASSERT_EQUAL_STRING(WIFI_SSID, doc[CONFIGURATION_FIELD_WIFI_SSID].as<String>().c_str());
    TEST_ASSERT_EQUAL_STRING(WIFI_SECRET, doc[CONFIGURATION_FIELD_WIFI_SECRET].as<String>().c_str());
    TEST_ASSERT_EQUAL_STRING(NTP_SERVER, doc[CONFIGURATION_FIELD_NTP_SERVER].as<String>().c_str());
    TEST_ASSERT_EQUAL(NTP_OFFSET, doc[CONFIGURATION_FIELD_NTP_OFFSET]);
    TEST_ASSERT_EQUAL(NTP_UPDATE_INTERVAL, doc[CONFIGURATION_FIELD_NTP_UPDATE_INTERVAL]);
}

int runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(test_constructor_default);
    RUN_TEST(test_constructor_withJsonDocument);
    RUN_TEST(test_toJsonDocument);
    UNITY_END();
    return 0;
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}
