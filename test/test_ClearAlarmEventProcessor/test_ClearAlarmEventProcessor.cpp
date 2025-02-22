#include "test_ClearAlarmEventProcessor.h"

void setUp(void) {
    clearAlarmEventProcessor = ClearAlarmEventProcessor();
    clearAlarmEventBuffer = ClearAlarmEventBuffer();
    systemStatus = SystemStatus();
}

void tearDown(void) {
}

void test_constructor() {
    TEST_ASSERT_EQUAL(0, clearAlarmEventBuffer.clearAlarmEventCount);
}

void test_processEvents_withNoEvents_withNoActiveAlarm() {
    clearAlarmEventProcessor.processEvents();

    TEST_ASSERT_EQUAL(0, clearAlarmEventBuffer.clearAlarmEventCount);
    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
}

void test_processEvents_withNoEvents_withActiveAlarm() {
    UnixTimeWithMilliSeconds expectedTimeStamp = UnixTimeWithMilliSeconds(123, 456);
    systemStatus.rebootDetected(expectedTimeStamp);

    clearAlarmEventProcessor.processEvents();

    TEST_ASSERT_EQUAL(1, systemStatus.getRebootCount());
    TEST_ASSERT_TRUE(expectedTimeStamp.equals(systemStatus.getLastRebootTimeStamp()));
    TEST_ASSERT_TRUE(systemStatus.isAlarmActive());
    TEST_ASSERT_EQUAL(0, clearAlarmEventBuffer.clearAlarmEventCount);
}

void test_processEvents_withEvents_withNoActiveAlarm() {
    clearAlarmEventBuffer.clearAlarmEventCount = 1;

    clearAlarmEventProcessor.processEvents();

    TEST_ASSERT_EQUAL(0, clearAlarmEventBuffer.clearAlarmEventCount);
    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
}

void test_processEvents_withEvents_withActiveAlarm() {
    UnixTimeWithMilliSeconds expectedTimeStamp = UnixTimeWithMilliSeconds(123, 456);
    systemStatus.rebootDetected(expectedTimeStamp);
    clearAlarmEventBuffer.clearAlarmEventCount = 1;

    clearAlarmEventProcessor.processEvents();

    TEST_ASSERT_EQUAL(0, clearAlarmEventBuffer.clearAlarmEventCount);
    TEST_ASSERT_FALSE(systemStatus.isAlarmActive());
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor);
    RUN_TEST(test_processEvents_withNoEvents_withNoActiveAlarm);
    RUN_TEST(test_processEvents_withNoEvents_withActiveAlarm);
    RUN_TEST(test_processEvents_withEvents_withNoActiveAlarm);
    RUN_TEST(test_processEvents_withEvents_withActiveAlarm);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {
}