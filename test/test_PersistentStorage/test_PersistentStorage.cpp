#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_PersistentStorage.h"

PersistentStorage storage;

void setUp(void) {
    storage = PersistentStorage();
    TEST_ASSERT_TRUE(storage.mount());
    if (storage.exists(TEST_DIRECTORY)) {
        TEST_ASSERT_TRUE(storage.recursiveRemovePath(String(TEST_DIRECTORY)));
    }
    TEST_ASSERT_TRUE(storage.createDirectory(TEST_DIRECTORY));
}

void tearDown(void) {
    storage.unMount();
}

void test_unMount() {
    storage.unMount();
    TEST_ASSERT_FALSE(storage.isMounted());
}

void test_constructor_default() {
    storage.unMount();
    storage = PersistentStorage();
    TEST_ASSERT_FALSE(storage.isMounted());
}

void test_mount() {
    storage.unMount();
    storage = PersistentStorage();
    TEST_ASSERT_TRUE(storage.mount());
}

void test_isMounted() {
    TEST_ASSERT_TRUE(storage.isMounted());
}

void test_mkdir() {
    String path = TEST_DIRECTORY + String("/mkdir-test");
    TEST_ASSERT_TRUE(storage.createDirectory(path));
    TEST_ASSERT_TRUE(storage.exists(path));
}

void test_removeDirectory() {
    String path = TEST_DIRECTORY + String("/rmdir-test");
    storage.createDirectory(path);
    storage.removeDirectory(path);
    TEST_ASSERT_FALSE(storage.exists(path));
}

void test_writeFile() {
    String path = TEST_DIRECTORY + String("/write-file-test.txt");
    String expectedContent = "Testing File Write";
    storage.writeFile(path, expectedContent);
    TEST_ASSERT_TRUE(storage.exists(path));
    TEST_ASSERT_TRUE(expectedContent.equals(storage.readFile(path)));

    expectedContent = "Test File Is Overwritten, Not Appended to";
    storage.writeFile(path, expectedContent);
    TEST_ASSERT_TRUE(expectedContent.equals(storage.readFile(path)));
}

void test_appendFile() {
    String path = TEST_DIRECTORY + String("/append-file-test.txt");
    String expectedContent = "Testing File Append";
    String appendContent = ", now appended to";

    storage.appendFile(path, expectedContent);
    TEST_ASSERT_TRUE(storage.exists(path));
    TEST_ASSERT_TRUE(expectedContent.equals(storage.readFile(path)));

    expectedContent += appendContent;
    storage.appendFile(path, appendContent);
    TEST_ASSERT_TRUE(expectedContent.equals(storage.readFile(path)));
}

void test_removeFile() {
    String path = TEST_DIRECTORY + String("/remove-file-test.txt");
    storage.writeFile(path, "content of Test File to be removed");
    storage.removeFile(path);
    TEST_ASSERT_FALSE(storage.exists(path));
}

void test_getLastModificationDate() {
    String path = TEST_DIRECTORY + String("/file-date-test.txt");
    rtcAdapter.setTime(RTCAdapter::ClockSource::FILE_DATE, UnixTimeWithMilliSeconds(1738130642, 0));
    storage.writeFile(path, "content of Test File to get last modification date");
    TEST_ASSERT_EQUAL(1738130642, storage.getLastModificationDate(path).getUnixTime());

    rtcAdapter.setTime(RTCAdapter::ClockSource::FILE_DATE, UnixTimeWithMilliSeconds(1738131234, 0));
    storage.appendFile(path, ", now appended to");
    TEST_ASSERT_EQUAL(1738131234, storage.getLastModificationDate(path).getUnixTime());
}

void test_getFreeSpace() {
    uint64_t totalSpace = storage.getTotalSpace();
    uint64_t usedSpace = storage.getUsedSpace();
    uint64_t expectedFreeSpace = totalSpace - usedSpace;

    TEST_ASSERT_GREATER_THAN(0, totalSpace);
    TEST_ASSERT_EQUAL(expectedFreeSpace, storage.getFreeSpace());
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
    RUN_TEST(test_mkdir);
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