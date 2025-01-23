#include "test_UnixTimeWithMilliSeconds.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_constructor_default() {
    UnixTimeWithMilliSeconds time = UnixTimeWithMilliSeconds(123, 456);
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

void test_greaterThan() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 457);
    TEST_ASSERT_TRUE(time2.greaterThan(time1));
    TEST_ASSERT_FALSE(time1.greaterThan(time2));
}

void test_greaterThan_withEquals() {
    UnixTimeWithMilliSeconds time1 = UnixTimeWithMilliSeconds(123, 456);
    UnixTimeWithMilliSeconds time2 = UnixTimeWithMilliSeconds(123, 456);
    TEST_ASSERT_FALSE(time1.greaterThan(time2));
    TEST_ASSERT_FALSE(time2.greaterThan(time1));
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

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor_default);
    RUN_TEST(test_equals);
    RUN_TEST(test_equals_withDifferent);
    RUN_TEST(test_greaterThan);
    RUN_TEST(test_greaterThan_withEquals);
    RUN_TEST(test_lessThan);
    RUN_TEST(test_lessThan_withEquals);

    RUN_TEST(test_getUnixTime);
    RUN_TEST(test_getMillis);
    RUN_TEST(test_getMillisAsZeroPaddedString);
    RUN_TEST(test_getFormattedTime);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}