#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#include "test_UnixTimeWithMilliSeconds.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_constructor_default() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds();
    TEST_ASSERT_EQUAL(0, time.getUnixTime());
    TEST_ASSERT_EQUAL(0, time.getMilliSeconds());
}

void test_constructor_withMilliSecondTimeStamp() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(123, 456);
    TEST_ASSERT_EQUAL(123, time.getUnixTime());
    TEST_ASSERT_EQUAL(456, time.getMilliSeconds());
}

void test_constructor_withJsonDocument() {
    JsonDocument doc = JsonDocument();
    doc[UNIX_TIME_FIELD_SECONDS] = 123;
    doc[UNIX_TIME_FIELD_MILLI_SECONDS] = 456;

    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(doc.as<JsonObject>());
    TEST_ASSERT_EQUAL(123, time.getUnixTime());
    TEST_ASSERT_EQUAL(456, time.getMilliSeconds());
}

void test_equals() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 456);
    TEST_ASSERT_TRUE(time1.equals(time2));
}

void test_equals_withDifferent() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 457);
    TEST_ASSERT_FALSE(time1.equals(time2));
}

void test_greaterThan_milliSecondDifference() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 457);
    TEST_ASSERT_TRUE(time2.greaterThan(time1));
    TEST_ASSERT_FALSE(time1.greaterThan(time2));
}

void test_greaterThan_lessThanOneSecondDifference() {
    auto time1 = UnixTimeWithMilliSeconds(123, 456);
    auto time2 = UnixTimeWithMilliSeconds(124, 7);
    TEST_ASSERT_TRUE(time2.greaterThan(time1));
    TEST_ASSERT_FALSE(time1.greaterThan(time2));
}

void test_greaterThan_withEquals() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 456);
    TEST_ASSERT_FALSE(time1.greaterThan(time2));
    TEST_ASSERT_FALSE(time2.greaterThan(time1));
}

void test_greaterOrEqual_withGreater() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 457);
    TEST_ASSERT_TRUE(time2.greaterOrEqual(time1));
    TEST_ASSERT_FALSE(time1.greaterOrEqual(time2));
}

void test_greaterOrEqual_withEquals() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 456);
    TEST_ASSERT_TRUE(time1.greaterOrEqual(time2));
    TEST_ASSERT_TRUE(time2.greaterOrEqual(time1));
}

void test_greaterOrEqual_withLess() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 455);
    TEST_ASSERT_TRUE(time1.greaterOrEqual(time2));
    TEST_ASSERT_FALSE(time2.greaterOrEqual(time1));
}

void test_lessThan() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 457);
    TEST_ASSERT_TRUE(time1.lessThan(time2));
    TEST_ASSERT_FALSE(time2.lessThan(time1));
}

void test_lessThan_withEquals() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 456);
    TEST_ASSERT_FALSE(time1.lessThan(time2));
    TEST_ASSERT_FALSE(time2.lessThan(time1));
}

void test_getUnixTime() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(1672531200, 456);
    TEST_ASSERT_EQUAL(1672531200, time.getUnixTime());
}

void test_getMillis() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(1672531200, 456);
    TEST_ASSERT_EQUAL(456, time.getMilliSeconds());
}

void test_getMillisAsZeroPaddedString() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(1672531200, 456);
    String millis = time.getMillisAsZeroPaddedString();
    TEST_ASSERT_EQUAL_STRING("456", millis.c_str());
}

void test_getFormattedTime() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(1672531200, 456);
    String formattedTime = time.getFormattedTime();
    TEST_ASSERT_EQUAL_STRING("2023-01-01 00:00:00", formattedTime.c_str());
}

void test_getDuration_withGreater_milliSecondDifference() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531200, 456);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531200, 457);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(1, duration.getSign());
    TEST_ASSERT_EQUAL(0, duration.getSeconds());
    TEST_ASSERT_EQUAL(1, duration.getMilliSeconds());
}

void test_getDuration_withGreater_withLessThanOneSecondDifference() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531200, 999);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531201, 123);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(1, duration.getSign());
    TEST_ASSERT_EQUAL(0, duration.getSeconds());
    TEST_ASSERT_EQUAL(124, duration.getMilliSeconds());
}

void test_getDuration_withGreater_withMoreThanOneSecondDifference() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531200, 456);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531201, 458);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(1, duration.getSign());
    TEST_ASSERT_EQUAL(1, duration.getSeconds());
    TEST_ASSERT_EQUAL(2, duration.getMilliSeconds());
}

void test_getDuration_withEqual() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531200, 456);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531200, 456);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(1, duration.getSign());
    TEST_ASSERT_EQUAL(0, duration.getSeconds());
    TEST_ASSERT_EQUAL(0, duration.getMilliSeconds());
}

void test_getDuration_withLess_milliSecondDifference() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531200, 457);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531200, 456);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(-1, duration.getSign());
    TEST_ASSERT_EQUAL(0, duration.getSeconds());
    TEST_ASSERT_EQUAL(1, duration.getMilliSeconds());
}

void test_getDuration_withLess_secondDifference() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531201, 200);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531200, 100);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(-1, duration.getSign());
    TEST_ASSERT_EQUAL(1, duration.getSeconds());
    TEST_ASSERT_EQUAL(100, duration.getMilliSeconds());
}

void test_getDuration_withLess_withLessThanOneSecondDifference() {
    UnixTimeWithMilliSeconds start = UnixTimeWithMilliSeconds(1672531201, 100);
    UnixTimeWithMilliSeconds end = UnixTimeWithMilliSeconds(1672531200, 999);

    auto duration = end.getDuration(start);

    TEST_ASSERT_EQUAL(-1, duration.getSign());
    TEST_ASSERT_EQUAL(0, duration.getSeconds());
    TEST_ASSERT_EQUAL(101, duration.getMilliSeconds());
}

void test_toJsonDocument() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(1672531200, 456);
    JsonDocument doc = time.toJsonDocument();

    TEST_ASSERT_EQUAL(1672531200, time.getUnixTime());
    TEST_ASSERT_EQUAL(1672531200, doc[UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(456, doc[UNIX_TIME_FIELD_MILLI_SECONDS]);
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor_default);
    RUN_TEST(test_constructor_withMilliSecondTimeStamp);
    RUN_TEST(test_constructor_withJsonDocument);

    RUN_TEST(test_equals);
    RUN_TEST(test_equals_withDifferent);

    RUN_TEST(test_greaterThan_milliSecondDifference);
    RUN_TEST(test_greaterThan_lessThanOneSecondDifference);
    RUN_TEST(test_greaterThan_withEquals);

    RUN_TEST(test_greaterOrEqual_withGreater);
    RUN_TEST(test_greaterOrEqual_withEquals);
    RUN_TEST(test_greaterOrEqual_withLess);

    RUN_TEST(test_lessThan);
    RUN_TEST(test_lessThan_withEquals);

    RUN_TEST(test_getUnixTime);
    RUN_TEST(test_getMillis);
    RUN_TEST(test_getMillisAsZeroPaddedString);
    RUN_TEST(test_getFormattedTime);

    RUN_TEST(test_getDuration_withGreater_milliSecondDifference);
    RUN_TEST(test_getDuration_withGreater_withLessThanOneSecondDifference);
    RUN_TEST(test_getDuration_withGreater_withMoreThanOneSecondDifference);
    RUN_TEST(test_getDuration_withEqual);
    RUN_TEST(test_getDuration_withLess_milliSecondDifference);
    RUN_TEST(test_getDuration_withLess_secondDifference);
    RUN_TEST(test_getDuration_withLess_withLessThanOneSecondDifference);

    RUN_TEST(test_toJsonDocument);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}

#pragma clang diagnostic pop