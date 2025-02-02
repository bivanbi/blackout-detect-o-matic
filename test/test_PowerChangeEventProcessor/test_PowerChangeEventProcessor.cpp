#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_PowerChangeEventProcessor.h"

void setUp(void) {
    systemStatus = SystemStatus();
    powerChangeEventProcessor = PowerChangeEventProcessor();
    powerChangeEventBuffer = PowerChangeEventBuffer();
}

void tearDown(void) {
}

void test_constructor() {
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);
}

void test_processEvents_withNoEvents_withPowerIsUp_withNoActiveBlackout() {
    powerChangeEventProcessor.processEvents(true);

    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);

    TEST_ASSERT_FALSE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_FALSE(systemStatus.getLastBlackout().isActive());
    TEST_ASSERT_EQUAL(0, systemStatus.getBlackoutCount());
}

void test_processEvents_withNoEvents_withPowerIsUp_withActiveBlackout() {
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(123, 456), false);

    powerChangeEventProcessor.processEvents(true);

    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);

    TEST_ASSERT_TRUE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_FALSE(systemStatus.getLastBlackout().isActive());
    TEST_ASSERT_EQUAL(1, systemStatus.getBlackoutCount());
}

/**
 * Theoretically, this should not happen, since an interrupt handler is
 * triggered by a change in the power state.
 */
void test_processEvents_withNoEvents_powerIsDown_withNoActiveBlackout() {
    powerChangeEventProcessor.processEvents(false);

    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);

    TEST_ASSERT_TRUE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_TRUE(systemStatus.getLastBlackout().isActive());
    TEST_ASSERT_EQUAL(1, systemStatus.getBlackoutCount());
}

void test_processEvents_withNoEvents_powerIsDown_withActiveBlackout() {
    systemStatus.setPowerStatus(UnixTimeWithMilliSeconds(123, 456), false);

    powerChangeEventProcessor.processEvents(false);

    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);

    TEST_ASSERT_TRUE(systemStatus.isBlackoutDetected());
    TEST_ASSERT_TRUE(systemStatus.getLastBlackout().isActive());
    TEST_ASSERT_EQUAL(1, systemStatus.getBlackoutCount());
}

void test_processEvents_withEvents_withPowerIsUp() {
    powerChangeEventBuffer.powerDownEventCount = 1;
    powerChangeEventBuffer.powerUpEventCount = 1;

    powerChangeEventProcessor.processEvents(true);

    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);
}

void test_processEvents_withEvents_withPowerIsDown() {
    powerChangeEventBuffer.powerUpEventCount = 1;
    powerChangeEventBuffer.powerUpEventCount = 1;

    powerChangeEventProcessor.processEvents(false);

    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerDownEventCount);
    TEST_ASSERT_EQUAL(0, powerChangeEventBuffer.powerUpEventCount);
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor);

    RUN_TEST(test_processEvents_withNoEvents_withPowerIsUp_withNoActiveBlackout);
    RUN_TEST(test_processEvents_withNoEvents_withPowerIsUp_withActiveBlackout);
    RUN_TEST(test_processEvents_withNoEvents_powerIsDown_withNoActiveBlackout);
    RUN_TEST(test_processEvents_withNoEvents_powerIsDown_withActiveBlackout);

    RUN_TEST(test_processEvents_withEvents_withPowerIsUp);
    RUN_TEST(test_processEvents_withEvents_withPowerIsDown);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {
    // delay(1000);
}

#pragma clang diagnostic pop