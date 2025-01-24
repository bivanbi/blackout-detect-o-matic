#include <unity.h>
#include <Arduino.h>

#include "RTCAdapter.h"
#include "UnixTimeWithMilliSeconds.h"

void setUp(void) {
    rtcAdapter = RTCAdapter();
}

void tearDown(void) {
}

void test_constructor_default() {
    RTCAdapter adapter = RTCAdapter(123);
    TEST_ASSERT_EQUAL(123, adapter.getOffset());
    TEST_ASSERT_EQUAL(0, rtcAdapter.getClockSource());
    TEST_ASSERT_LESS_THAN(10000, rtcAdapter.getUnixTime());
}

void test_setTime() {
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, 1672531200);
    TEST_ASSERT_GREATER_OR_EQUAL(1672531200, rtcAdapter.getUnixTime());
    TEST_ASSERT_EQUAL(RTCAdapter::ClockSource::NTP, rtcAdapter.getClockSource());
}

void test_getTime() {
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, 1672531200, 456);
    UnixTimeWithMilliSeconds time = rtcAdapter.getTime();
    TEST_ASSERT_EQUAL(1672531200, time.getUnixTime());
    TEST_ASSERT_EQUAL(456, time.getMilliSeconds());
}

void test_getMillis() {
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, 1672531200, 456);
    unsigned long millis1 = rtcAdapter.getMillis();
    delay(2);
    unsigned long millis2 = rtcAdapter.getMillis();
    TEST_ASSERT_EQUAL(456, millis1);
    TEST_ASSERT_EQUAL(457, millis2);
}

void test_getMicros() {
    unsigned long micros1 = rtcAdapter.getMicros();
    delay(2);
    unsigned long micros2 = rtcAdapter.getMicros();
    TEST_ASSERT_GREATER_THAN(micros1, micros2);
    TEST_ASSERT_GREATER_THAN(1000, micros2 - micros1);
    TEST_ASSERT_LESS_THAN(3000, micros2 - micros1);
}

void test_isTimeSet_withNTP() {
    TEST_ASSERT_FALSE(rtcAdapter.isTimeSet());
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, 1672531200);
    TEST_ASSERT_TRUE(rtcAdapter.isTimeSet());
}

void test_isTimeSet_withFileDate() {
    TEST_ASSERT_FALSE(rtcAdapter.isTimeSet());
    rtcAdapter.setTime(RTCAdapter::ClockSource::FILE_DATE, 1672531200);
    TEST_ASSERT_TRUE(rtcAdapter.isTimeSet());
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor_default);
    RUN_TEST(test_setTime);
    RUN_TEST(test_getTime);
    RUN_TEST(test_getMillis);
    RUN_TEST(test_getMicros);
    RUN_TEST(test_isTimeSet_withNTP);
    RUN_TEST(test_isTimeSet_withFileDate);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}