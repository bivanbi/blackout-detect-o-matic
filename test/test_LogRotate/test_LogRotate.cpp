#include "test_LogRotate.h"

void setUp(void) {
    configuration.setLogDirectory(TEST_LOG_DIRECTORY);
    configuration.setLogFileName(TEST_LOG_FILE_NAME);

    if (!persistentStorage.isMounted()) {
        persistentStorage.mount();
    }

    if (!persistentStorage.exists(TEST_DIRECTORY)) {
        persistentStorage.createDirectory(TEST_DIRECTORY);
    }

    if (persistentStorage.exists(TEST_LOG_DIRECTORY)) {
        persistentStorage.recursiveRemovePath(TEST_LOG_DIRECTORY);
    }

    persistentStorage.createDirectory(TEST_LOG_DIRECTORY);
}

void tearDown(void) {}

void test_getFilePath() {
    TEST_ASSERT_EQUAL_STRING(TEST_LOG_FILE_PATH, LogRotate::getFilePath().c_str());
    TEST_ASSERT_EQUAL_STRING(TEST_LOG_FILE_PATH ".1" , LogRotate::getFilePath(1).c_str());
    TEST_ASSERT_EQUAL_STRING(TEST_LOG_FILE_PATH ".2" , LogRotate::getFilePath(2).c_str());
    TEST_ASSERT_EQUAL_STRING(TEST_LOG_FILE_PATH ".30" , LogRotate::getFilePath(30).c_str());
}

void test_shouldRotate() {
    String expectedContent = "test logrotate should rotate";
    int fileSize = expectedContent.length();
    TEST_ASSERT_TRUE(persistentStorage.writeFile(TEST_LOG_FILE_PATH, expectedContent));
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(TEST_LOG_FILE_PATH).c_str());
    TEST_ASSERT_EQUAL(fileSize, persistentStorage.getSize(TEST_LOG_FILE_PATH));

    configuration.setLogRotateMinimumFileSize(fileSize + 1);
    TEST_ASSERT_FALSE(LogRotate::shouldRotate());

    configuration.setLogRotateMinimumFileSize(fileSize);
    TEST_ASSERT_TRUE(LogRotate::shouldRotate());

    configuration.setLogRotateMinimumFileSize(fileSize - 1);
    TEST_ASSERT_TRUE(LogRotate::shouldRotate());
}

void test_rotate() {
    String expectedContent = "test logfile before logrotate";
    int expectedFileSize = expectedContent.length();
    TEST_ASSERT_TRUE(persistentStorage.writeFile(TEST_LOG_FILE_PATH, expectedContent));

    configuration.setLogRotateMinimumFileSize(expectedFileSize + 1);
    LogRotate::rotate();
    TEST_ASSERT_TRUE(persistentStorage.exists(TEST_LOG_FILE_PATH));
    TEST_ASSERT_FALSE(persistentStorage.exists(TEST_LOG_FILE_PATH ".1"));
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(TEST_LOG_FILE_PATH).c_str());

    configuration.setLogRotateMinimumFileSize(expectedFileSize);
    LogRotate::rotate();
    TEST_ASSERT_FALSE(persistentStorage.exists(TEST_LOG_FILE_PATH));
    TEST_ASSERT_TRUE(persistentStorage.exists(TEST_LOG_FILE_PATH ".1"));
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(TEST_LOG_FILE_PATH ".1").c_str());
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_getFilePath);
    RUN_TEST(test_shouldRotate);
    RUN_TEST(test_rotate);

    return UNITY_END();
}

void setup() {
    delay(2000); // service delay
    runUnityTests();
}

void loop() {}
