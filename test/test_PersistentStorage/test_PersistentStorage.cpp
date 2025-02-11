#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_PersistentStorage.h"

void setUp(void) {
    persistentStorage = PersistentStorage();
    TEST_ASSERT_TRUE(persistentStorage.mount());
    if (persistentStorage.exists(TEST_DIRECTORY)) {
        TEST_ASSERT_TRUE(persistentStorage.recursiveRemovePath(String(TEST_DIRECTORY)));
    }
    TEST_ASSERT_TRUE(persistentStorage.createDirectory(TEST_DIRECTORY));
}

void tearDown(void) {
    persistentStorage.unMount();
}

void test_unMount() {
    persistentStorage.unMount();
    TEST_ASSERT_FALSE(persistentStorage.isMounted());
}

void test_constructor_default() {
    persistentStorage.unMount();
    TEST_ASSERT_FALSE(persistentStorage.isMounted());
}

void test_mount() {
    persistentStorage.unMount();
    TEST_ASSERT_TRUE(persistentStorage.mount());
}

void test_isMounted() {
    TEST_ASSERT_TRUE(persistentStorage.isMounted());
}

void test_createDirectory() {
    String path = TEST_DIRECTORY + String("/mkdir-test");
    TEST_ASSERT_TRUE(persistentStorage.createDirectory(path));
    TEST_ASSERT_TRUE(persistentStorage.exists(path));
}

void test_removeDirectory() {
    String path = TEST_DIRECTORY + String("/rmdir-test");
    persistentStorage.createDirectory(path);
    persistentStorage.removeDirectory(path);
    TEST_ASSERT_FALSE(persistentStorage.exists(path));
}

void test_writeFile() {
    String path = TEST_DIRECTORY + String("/write-file-test.txt");
    String expectedContent = "Testing File Write";
    persistentStorage.writeFile(path, expectedContent);
    TEST_ASSERT_TRUE(persistentStorage.exists(path));
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(path).c_str());

    expectedContent = "Test File Is Overwritten, Not Appended to";
    persistentStorage.writeFile(path, expectedContent);
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(path).c_str());
}

void test_appendFile() {
    String path = TEST_DIRECTORY + String("/append-file-test.txt");
    String expectedContent = "Testing File Append";
    String appendContent = ", now appended to";

    persistentStorage.appendFile(path, expectedContent);
    TEST_ASSERT_TRUE(persistentStorage.exists(path));
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(path).c_str());

    expectedContent += appendContent;
    persistentStorage.appendFile(path, appendContent);
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(path).c_str());
}

void test_removeFile() {
    String path = TEST_DIRECTORY + String("/remove-file-test.txt");
    persistentStorage.writeFile(path, "content of Test File to be removed");
    persistentStorage.removeFile(path);
    TEST_ASSERT_FALSE(persistentStorage.exists(path));
}

void test_getLastModificationDate() {
    rtcAdapter.pause();
    String path = TEST_DIRECTORY + String("/file-date-test.txt");
    rtcAdapter.setTime(RTCAdapter::ClockSource::FILE_DATE, UnixTimeWithMilliSeconds(1738130642, 0));
    persistentStorage.writeFile(path, "content of Test File to get last modification date");
    TEST_ASSERT_EQUAL(1738130642, persistentStorage.getLastModificationDate(path).getUnixTime());

    rtcAdapter.setTime(RTCAdapter::ClockSource::FILE_DATE, UnixTimeWithMilliSeconds(1738131234, 0));
    persistentStorage.appendFile(path, ", now appended to");
    TEST_ASSERT_EQUAL(1738131234, persistentStorage.getLastModificationDate(path).getUnixTime());
}

void test_getFreeSpace() {
    uint64_t totalSpace = persistentStorage.getTotalSpace();
    uint64_t usedSpace = persistentStorage.getUsedSpace();
    uint64_t expectedFreeSpace = totalSpace - usedSpace;

    TEST_ASSERT_GREATER_THAN(0, totalSpace);
    TEST_ASSERT_EQUAL(expectedFreeSpace, persistentStorage.getFreeSpace());
}

void listDir(String dirname, uint8_t levels) {
    Serial.println("Listing directory: " + dirname);

    File root = SD_MMC.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(file.path(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_unMount);
    RUN_TEST(test_constructor_default);
    RUN_TEST(test_mount);
    RUN_TEST(test_isMounted);
    RUN_TEST(test_createDirectory);
    RUN_TEST(test_removeDirectory);

    RUN_TEST(test_writeFile);
    RUN_TEST(test_appendFile);
    RUN_TEST(test_removeFile);

    RUN_TEST(test_getLastModificationDate);
    RUN_TEST(test_getFreeSpace);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}

#pragma clang diagnostic pop