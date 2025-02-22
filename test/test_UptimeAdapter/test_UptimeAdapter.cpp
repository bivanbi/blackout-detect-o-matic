#include "test_UptimeAdapter.h"

void setUp(void) {
    UptimeAdapter::returnToRealUptime();
}

void tearDown(void) {
}

void test_get(void) {
    delay(17);
    TEST_ASSERT_GREATER_OR_EQUAL(17, UptimeAdapter::get());
    TEST_ASSERT_LESS_OR_EQUAL(millis(), UptimeAdapter::get());
}

void test_pause(void) {
    unsigned long uptimeAtPause = UptimeAdapter::pause();
    TEST_ASSERT_LESS_OR_EQUAL(millis(), uptimeAtPause);

    delay(100);

    TEST_ASSERT_EQUAL(uptimeAtPause, UptimeAdapter::get());
}

void test_set(void) {
    unsigned long uptimeAtSet = UptimeAdapter::set(100);
    long differenceBeforeDelay = uptimeAtSet - 100;

    delay(100);

    unsigned long uptimeAfterDelay = UptimeAdapter::get();
    long differenceAfterDelay = millis() - uptimeAfterDelay;

    TEST_ASSERT_UINT32_WITHIN(5, 200, uptimeAfterDelay);
    TEST_ASSERT_EQUAL(differenceBeforeDelay, differenceAfterDelay);
}

void test_set_whenPaused(void) {
    UptimeAdapter::pause();
    UptimeAdapter::set(123);
    TEST_ASSERT_EQUAL(123, UptimeAdapter::get());
}

void test_resume(void) {
    UptimeAdapter::pause();
    delay(100);
    UptimeAdapter::resume();

    TEST_ASSERT_UINT32_WITHIN(5, millis() - 100, UptimeAdapter::get());
}

void test_returnToRealUptime(void) {
    UptimeAdapter::pause();
    UptimeAdapter::set(100);
    UptimeAdapter::returnToRealUptime();

    delay(100);

    TEST_ASSERT_GREATER_OR_EQUAL(100, UptimeAdapter::get());
    TEST_ASSERT_UINT32_WITHIN(5, millis(), UptimeAdapter::get());
}

void test_getFormatterUptime(void) {
    UptimeAdapter::pause();
    UptimeAdapter::set(187748257);
    TEST_ASSERT_EQUAL_STRING("2 days 04:09:08.257", UptimeAdapter::getFormattedUptime().c_str());
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_get);
    RUN_TEST(test_pause);
    RUN_TEST(test_set);
    RUN_TEST(test_set_whenPaused);
    RUN_TEST(test_resume);
    RUN_TEST(test_returnToRealUptime);
    RUN_TEST(test_getFormatterUptime);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {
}
