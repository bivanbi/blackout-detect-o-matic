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

void test_isDirectory_withDirectory() {
    String path = TEST_DIRECTORY + String("/isdir-test");
    persistentStorage.createDirectory(path);
    TEST_ASSERT_TRUE(persistentStorage.isDirectory(path));
}

void test_isDirectory_withFile() {
    String path = TEST_DIRECTORY + String("/isdir-test");
    persistentStorage.writeFile(path, "content of Test File to check if it is a directory");
    TEST_ASSERT_FALSE(persistentStorage.isDirectory(path));
}

void test_listDirectory() {
    persistentStorage.writeFile(String(TEST_DIRECTORY) + "/list-directory-test-file.txt", "content of Test File to get last modification date");

    String result = persistentStorage.listDirectory("/");

    TEST_ASSERT_GREATER_OR_EQUAL(0, result.indexOf("d "));
    TEST_ASSERT_GREATER_OR_EQUAL(0, result.indexOf("f "));
    TEST_ASSERT_GREATER_OR_EQUAL(0, result.indexOf(String(TEST_DIRECTORY) + "/list-directory-test-file.txt"));
}

void test_listDirectory_withNonExistentDirectory() {
    String path = "/non-existent-directory";
    String result = persistentStorage.listDirectory(path);

    TEST_ASSERT_GREATER_OR_EQUAL(0, result.indexOf("Failed to open directory"));
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

void test_rename() {
    String path = TEST_DIRECTORY + String("/rename-file-test.txt");
    String newPath = TEST_DIRECTORY + String("/renamed-file-test.txt");
    persistentStorage.writeFile(path, "content of Test File to be renamed");

    persistentStorage.rename(path, newPath);

    TEST_ASSERT_FALSE(persistentStorage.exists(path));
    TEST_ASSERT_TRUE(persistentStorage.exists(newPath));
    TEST_ASSERT_EQUAL_STRING("content of Test File to be renamed", persistentStorage.readFile(newPath).c_str());
}

void test_getSize_withDirectory() {
    if (!persistentStorage.exists(TEST_DIRECTORY + String("/directory-size-test"))) {
        persistentStorage.createDirectory(TEST_DIRECTORY + String("/directory-size-test"));
    }

    String path = TEST_DIRECTORY + String("/directory-size-test");
    persistentStorage.createDirectory(path);
    TEST_ASSERT_EQUAL(0, persistentStorage.getSize(path));
}

void test_getSize_withFile() {
    String path = TEST_DIRECTORY + String("/file-size-test.txt");
    String expectedContent = "content of Test File to get size goo";
    int expectedFileSize = expectedContent.length(); // account for Unicode encoding
    persistentStorage.writeFile(path, expectedContent);
    TEST_ASSERT_EQUAL_STRING(expectedContent.c_str(), persistentStorage.readFile(path).c_str());
    TEST_ASSERT_EQUAL(expectedFileSize, persistentStorage.getSize(path));
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

int runUnityTests(void) {
    UNITY_BEGIN();

//    RUN_TEST(test_unMount);
//    RUN_TEST(test_constructor_default);
//    RUN_TEST(test_mount);
//    RUN_TEST(test_isMounted);
//    RUN_TEST(test_createDirectory);
//    RUN_TEST(test_removeDirectory);
//    RUN_TEST(test_isDirectory_withDirectory);
//    RUN_TEST(test_isDirectory_withFile);
//    RUN_TEST(test_listDirectory);
//    RUN_TEST(test_listDirectory_withNonExistentDirectory);
//
//    RUN_TEST(test_writeFile);
//    RUN_TEST(test_appendFile);
//    RUN_TEST(test_removeFile);
//    RUN_TEST(test_rename);
//    RUN_TEST(test_getSize_withDirectory);
    RUN_TEST(test_getSize_withFile);

//    RUN_TEST(test_getLastModificationDate);
//    RUN_TEST(test_getFreeSpace);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}

#pragma clang diagnostic pop