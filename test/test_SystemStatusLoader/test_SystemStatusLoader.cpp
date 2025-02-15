#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#include "test_SystemStatusLoader.h"

String testFilePath = TEST_DIRECTORY "/systemStatus.json";

void setUp(void) {
    configuration.setSystemStatusFilePath(testFilePath);
    TEST_ASSERT_EQUAL_STRING(testFilePath.c_str(), configuration.getSystemStatusFilePath().c_str());
    TEST_ASSERT_TRUE(persistentStorage.mount());
    if (!persistentStorage.exists(TEST_DIRECTORY)) {
        TEST_ASSERT_TRUE(persistentStorage.createDirectory(TEST_DIRECTORY));
    }
    if (persistentStorage.exists(configuration.getSystemStatusFilePath())) {
        TEST_ASSERT_TRUE(persistentStorage.removeFile(configuration.getSystemStatusFilePath()));
    }
}

void tearDown(void) {
    persistentStorage.unMount();
}

void test_load_withStatusFileMissing() {
    TEST_ASSERT_FALSE(SystemStatusLoader::load());
}

void test_load_withCorruptStatusFile() {
    persistentStorage.writeFile(testFilePath, "corrupt content");
    TEST_ASSERT_EQUAL_STRING("corrupt content", persistentStorage.readFile(testFilePath).c_str());
    TEST_ASSERT_FALSE(SystemStatusLoader::load());
}

void test_load_withValidStatusFile() {
    SystemStatus expectedStatus;
    systemStatus.reset(UnixTimeWithMilliSeconds(123, 567));
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(1234, 567), false);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(1345, 123), true);
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(2345, 678));

    persistentStorage.writeFile(testFilePath, expectedStatus.toJsonDocument().as<String>());
    TEST_ASSERT_EQUAL_STRING(expectedStatus.toJsonDocument().as<String>().c_str(), persistentStorage.readFile(testFilePath).c_str());

    TEST_ASSERT_TRUE(SystemStatusLoader::load());
    TEST_ASSERT_EQUAL(expectedStatus.getBlackoutCount(), systemStatus.getBlackoutCount());
    TEST_ASSERT_TRUE(expectedStatus.getLastBlackout().equals(systemStatus.getLastBlackout()));
    TEST_ASSERT_TRUE(expectedStatus.getLongestBlackout().equals(systemStatus.getLongestBlackout()));
    TEST_ASSERT_TRUE(expectedStatus.getShortestBlackout().equals(systemStatus.getShortestBlackout()));
    TEST_ASSERT_EQUAL(expectedStatus.getRebootCount(), systemStatus.getRebootCount());
    TEST_ASSERT_TRUE(expectedStatus.getLastRebootTimeStamp().equals(systemStatus.getLastRebootTimeStamp()));
    TEST_ASSERT_TRUE(expectedStatus.getLastResetTimeStamp().equals(systemStatus.getLastResetTimeStamp()));
}

void test_save() {
    SystemStatus expectedStatus;
    expectedStatus.reset(UnixTimeWithMilliSeconds(3123, 567));
    expectedStatus.setPowerStatus(UnixTimeWithMilliSeconds(31234, 567), false);
    expectedStatus.setPowerStatus(UnixTimeWithMilliSeconds(31345, 123), true);
    expectedStatus.rebootDetected(UnixTimeWithMilliSeconds(32345, 678));

    systemStatus.reset(UnixTimeWithMilliSeconds(3123, 567));
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(31234, 567), false);
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(31345, 123), true);
    systemStatus.rebootDetected(UnixTimeWithMilliSeconds(32345, 678));

    TEST_ASSERT_TRUE(SystemStatusLoader::save());
    TEST_ASSERT_EQUAL_STRING(expectedStatus.toJsonDocument().as<String>().c_str(), persistentStorage.readFile(testFilePath).c_str());
}

int runUnitTests() {
    UNITY_BEGIN();

    RUN_TEST(test_load_withStatusFileMissing);
    RUN_TEST(test_load_withCorruptStatusFile);
    RUN_TEST(test_load_withValidStatusFile);

    RUN_TEST(test_save);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnitTests();
}

void loop() {
}

#pragma clang diagnostic pop