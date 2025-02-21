#include "test_PersistentLogger.h"

void setUp() {
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
    PersistentLogger::begin();
}

void tearDown() {
}

void test_logFilePath() {
    TEST_ASSERT_EQUAL_STRING(TEST_LOG_FILE_NAME, configuration.getLogFileName().c_str());
    TEST_ASSERT_EQUAL_STRING(TEST_LOG_FILE_PATH, PersistentLogger::getLogFilePath().c_str());
}

void test_log_defaultLevel() {
    PersistentLogger::log(LogSeverity::Severity::ERROR, "Test error message");
    PersistentLogger::log(LogSeverity::Severity::INFO, "Test info message");
    PersistentLogger::log(LogSeverity::Severity::DEBUG, "Test debug message");

    String logFileContent = persistentStorage.readFile(TEST_LOG_FILE_PATH);

    TEST_ASSERT_TRUE(logFileContent.indexOf("ERROR") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("Test error message") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("INFO") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("Test info message") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("DEBUG") < 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("Test debug message") < 0);
}

void test_setLoglevel() {
    PersistentLogger::setLogLevel(LogSeverity::Severity::DEBUG);
    PersistentLogger::log(LogSeverity::Severity::DEBUG, "Test debug message");
    PersistentLogger::log(LogSeverity::Severity::ERROR, "Test error message");

    String logFileContent = persistentStorage.readFile(TEST_LOG_FILE_PATH);

    TEST_ASSERT_TRUE(logFileContent.indexOf("ERROR") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("Test error message") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("DEBUG") > 0);
    TEST_ASSERT_TRUE(logFileContent.indexOf("Test debug message") > 0);
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_logFilePath);
    RUN_TEST(test_log_defaultLevel);
    RUN_TEST(test_setLoglevel);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
};

void loop() {};
