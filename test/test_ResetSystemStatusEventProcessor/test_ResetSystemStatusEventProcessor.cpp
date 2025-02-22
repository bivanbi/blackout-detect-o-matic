#include "test_ResetSystemStatusEventProcessor.h"

void setUp(void) {
    resetSystemStatusEventProcessor = ResetSystemStatusEventProcessor();
    resetSystemStatusEventBuffer = ResetSystemStatusEventBuffer();
    systemStatus = SystemStatus();
}

void tearDown(void) {
}

void test_constructor() {
    TEST_ASSERT_EQUAL(0, resetSystemStatusEventBuffer.resetEventCount);
    TEST_ASSERT_EQUAL(0, systemStatus.getLastResetTimeStamp().getUnixTime());
    TEST_ASSERT_EQUAL(0, systemStatus.getLastResetTimeStamp().getMilliSeconds());
}

void test_processEvents_withNoEvents() {
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, UnixTimeWithMilliSeconds(1234, 1));

    resetSystemStatusEventProcessor.processEvents();

    TEST_ASSERT_EQUAL(0, resetSystemStatusEventBuffer.resetEventCount);
    TEST_ASSERT_EQUAL(0, systemStatus.getLastResetTimeStamp().getUnixTime());
}

void test_processEvents_withEvents() {
    rtcAdapter.pause();
    rtcAdapter.setTime(RTCAdapter::ClockSource::NTP, UnixTimeWithMilliSeconds(1234, 1));
    resetSystemStatusEventBuffer.resetEventCount = 1;

    resetSystemStatusEventProcessor.processEvents();

    TEST_ASSERT_EQUAL(0, resetSystemStatusEventBuffer.resetEventCount);
    TEST_ASSERT_EQUAL(1234, systemStatus.getLastResetTimeStamp().getUnixTime());
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor);
    RUN_TEST(test_processEvents_withNoEvents);
    RUN_TEST(test_processEvents_withEvents);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {
}