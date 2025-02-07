#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#include "test_Duration.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_constructor(void) {
    Duration duration = Duration(1, 2, 3);
    TEST_ASSERT_EQUAL(1, duration.getSign());
    TEST_ASSERT_EQUAL(2, duration.getSeconds());
    TEST_ASSERT_EQUAL(3, duration.getMilliSeconds());
}

void test_greaterThan_withEquals(void) {
    Duration duration1 = Duration(1, 2, 3);
    Duration duration2 = Duration(1, 2, 3);
    TEST_ASSERT_FALSE(duration1.greaterThan(duration2));
}

void test_greaterThan_withMilliSecondDifference(void) {
    Duration duration1 = Duration(1, 2, 3);
    Duration duration2 = Duration(1, 2, 2);
    TEST_ASSERT_TRUE(duration1.greaterThan(duration2));
}

void test_greaterThan_withLessThanOneSecondDifference(void) {
    Duration duration1 = Duration(1, 2, 3);
    Duration duration2 = Duration(1, 1, 4);
    TEST_ASSERT_TRUE(duration1.greaterThan(duration2));
}

void test_greaterThan_withSecondDifference(void) {
    Duration duration1 = Duration(1, 2, 3);
    Duration duration2 = Duration(1, 1, 300);
    TEST_ASSERT_TRUE(duration1.greaterThan(duration2));
}

void test_greaterThan_withLesser_withEquals(void) {
    Duration duration1 = Duration(1, 2, 3);
    Duration duration2 = Duration(1, 2, 3);
    TEST_ASSERT_FALSE(duration1.lessThan(duration2));
}

void test_greaterThan_withLesser_withMilliSecondDifference(void) {
    Duration duration1 = Duration(1, 2, 2);
    Duration duration2 = Duration(1, 2, 3);
    TEST_ASSERT_TRUE(duration1.lessThan(duration2));
}

void test_greaterThan_withLesser_withLessThanOneSecondDifference(void) {
    Duration duration1 = Duration(1, 1, 4);
    Duration duration2 = Duration(1, 2, 3);
    TEST_ASSERT_TRUE(duration1.lessThan(duration2));
}

void test_greaterThan_withLesser_withSecondDifference(void) {
    Duration duration1 = Duration(1, 1, 300);
    Duration duration2 = Duration(1, 2, 3);
    TEST_ASSERT_TRUE(duration1.lessThan(duration2));
}

void test_getFormattedDuration(void) {
    Duration duration = Duration(1, 14948, 25);
    TEST_ASSERT_EQUAL_STRING("04:09:08.025", duration.getFormattedDuration().c_str());
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor);

    RUN_TEST(test_greaterThan_withEquals);
    RUN_TEST(test_greaterThan_withMilliSecondDifference);
    RUN_TEST(test_greaterThan_withLessThanOneSecondDifference);
    RUN_TEST(test_greaterThan_withSecondDifference);

    RUN_TEST(test_greaterThan_withLesser_withEquals);
    RUN_TEST(test_greaterThan_withLesser_withMilliSecondDifference);
    RUN_TEST(test_greaterThan_withLesser_withLessThanOneSecondDifference);
    RUN_TEST(test_greaterThan_withLesser_withSecondDifference);

    RUN_TEST(test_getFormattedDuration);
    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}

#pragma clang diagnostic pop