#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"

#include "test_Blackout.h"

Blackout blackout;

void setUp(void) {
    blackout = Blackout();
}

void tearDown(void) {
}

void test_constructor_default() {
    TEST_ASSERT_EQUAL(0, blackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(0, blackout.getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(0, blackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(0, blackout.getEnd().getMilliSeconds());
}

void test_constructor_initStartTime() {
    blackout = Blackout(UnixTimeWithMilliSeconds(123456789, 432));

    TEST_ASSERT_EQUAL(123456789, blackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(432, blackout.getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(0, blackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(0, blackout.getEnd().getMilliSeconds());
}

void test_constructor_initStartAndEndTime() {
    blackout = Blackout(UnixTimeWithMilliSeconds(100, 300), UnixTimeWithMilliSeconds(233, 456));

    TEST_ASSERT_FALSE(blackout.isActive());
    TEST_ASSERT_EQUAL(100, blackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(300, blackout.getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(233, blackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(456, blackout.getEnd().getMilliSeconds());
}

void test_constructor_withJsonDocument() {
    JsonDocument doc;
    doc[BLACKOUT_FIELD_START] = UnixTimeWithMilliSeconds(100, 200).toJsonDocument();
    doc[BLACKOUT_FIELD_END] = UnixTimeWithMilliSeconds(300, 400).toJsonDocument();

    blackout = Blackout(doc);

    TEST_ASSERT_EQUAL(100, blackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(200, blackout.getStart().getMilliSeconds());
    TEST_ASSERT_EQUAL(300, blackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(400, blackout.getEnd().getMilliSeconds());
}

void test_setStart() {
    blackout.setStart(UnixTimeWithMilliSeconds(123456789, 321));

    TEST_ASSERT_EQUAL(123456789, blackout.getStart().getUnixTime());
    TEST_ASSERT_EQUAL(321, blackout.getStart().getMilliSeconds());
}

void test_setEnd() {
    blackout.setEnd(UnixTimeWithMilliSeconds(123456789, 321));

    TEST_ASSERT_EQUAL(123456789, blackout.getEnd().getUnixTime());
    TEST_ASSERT_EQUAL(321, blackout.getEnd().getMilliSeconds());
}

void test_isActive_withNoStart_withNoEnd() {
    TEST_ASSERT_FALSE(blackout.isActive());
}

void test_isActive_withNoStart() {
    blackout.setEnd(UnixTimeWithMilliSeconds(200, 200));

    TEST_ASSERT_FALSE(blackout.isActive());
}

void test_isActive_withNoEnd() {
    blackout.setStart(UnixTimeWithMilliSeconds(100, 100));

    TEST_ASSERT_TRUE(blackout.isActive());
}

void test_isActive_withEndTimeBeforeStartTime() {
    blackout.setStart(UnixTimeWithMilliSeconds(200, 200));
    blackout.setEnd(UnixTimeWithMilliSeconds(100, 100));

    TEST_ASSERT_FALSE(blackout.isActive());
}

void test_isActive_withStart_withEnd() {
    blackout.setStart(UnixTimeWithMilliSeconds(100, 100));
    blackout.setEnd(UnixTimeWithMilliSeconds(200, 200));

    TEST_ASSERT_FALSE(blackout.isActive());
}

void test_isValid_withNoStart_withNoEnd() {
    TEST_ASSERT_FALSE(blackout.isValid());
}

void test_isValid_withNoStart() {
    blackout.setEnd(UnixTimeWithMilliSeconds(200, 200));

    TEST_ASSERT_FALSE(blackout.isValid());
}

void test_isValid_withNoEnd() {
    blackout.setStart(UnixTimeWithMilliSeconds(100, 100));

    TEST_ASSERT_FALSE(blackout.isValid());
}

void test_isValid_withEndTimeBeforeStartTime() {
    blackout.setStart(UnixTimeWithMilliSeconds(200, 200));
    blackout.setEnd(UnixTimeWithMilliSeconds(100, 100));

    TEST_ASSERT_FALSE(blackout.isValid());
}

void test_isValid_withStart_withEnd() {
    blackout.setStart(UnixTimeWithMilliSeconds(100, 100));
    blackout.setEnd(UnixTimeWithMilliSeconds(200, 200));

    TEST_ASSERT_TRUE(blackout.isValid());
}

void test_getDuration_withNoStart_withNoEnd() {
    TEST_ASSERT_EQUAL(1, blackout.getDuration().getSign());
    TEST_ASSERT_EQUAL(0, blackout.getDuration().getSeconds());
    TEST_ASSERT_EQUAL(0, blackout.getDuration().getMilliSeconds());
}

void test_getDuration_withNoStart() {
    blackout.setEnd(UnixTimeWithMilliSeconds(200, 300));

    TEST_ASSERT_EQUAL(1, blackout.getDuration().getSign());
    TEST_ASSERT_EQUAL(200, blackout.getDuration().getSeconds());
    TEST_ASSERT_EQUAL(300, blackout.getDuration().getMilliSeconds());
}

void test_getDuration_withNoEnd() {
    blackout.setStart(UnixTimeWithMilliSeconds(200, 300));

    TEST_ASSERT_EQUAL(-1, blackout.getDuration().getSign());
    TEST_ASSERT_EQUAL(200, blackout.getDuration().getSeconds());
    TEST_ASSERT_EQUAL(300, blackout.getDuration().getMilliSeconds());
}

void test_getDuration_withEndTimeBeforeStartTime() {
    blackout.setStart(UnixTimeWithMilliSeconds(300, 400));
    blackout.setEnd(UnixTimeWithMilliSeconds(100, 200));

    TEST_ASSERT_EQUAL(-1, blackout.getDuration().getSign());
    TEST_ASSERT_EQUAL(200, blackout.getDuration().getSeconds());
    TEST_ASSERT_EQUAL(200, blackout.getDuration().getMilliSeconds());
}

void test_getDuration_withStart_withEnd() {
    blackout.setStart(UnixTimeWithMilliSeconds(100, 200));
    blackout.setEnd(UnixTimeWithMilliSeconds(300, 400));

    TEST_ASSERT_EQUAL(1, blackout.getDuration().getSign());
    TEST_ASSERT_EQUAL(200, blackout.getDuration().getSeconds());
    TEST_ASSERT_EQUAL(200, blackout.getDuration().getMilliSeconds());
}

void test_toJsonDocument() {
    blackout = Blackout(UnixTimeWithMilliSeconds(100, 200));
    blackout.setEnd(UnixTimeWithMilliSeconds(300, 400));

    JsonDocument doc = blackout.toJsonDocument();
    TEST_ASSERT_EQUAL(100, doc[BLACKOUT_FIELD_START][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(200, doc[BLACKOUT_FIELD_START][UNIX_TIME_FIELD_MILLI_SECONDS]);
    TEST_ASSERT_EQUAL(300, doc[BLACKOUT_FIELD_END][UNIX_TIME_FIELD_SECONDS]);
    TEST_ASSERT_EQUAL(400, doc[BLACKOUT_FIELD_END][UNIX_TIME_FIELD_MILLI_SECONDS]);
}

int runUnityTests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_constructor_default);
    RUN_TEST(test_constructor_initStartTime);
    RUN_TEST(test_constructor_initStartAndEndTime);
    RUN_TEST(test_constructor_withJsonDocument);

    RUN_TEST(test_setStart);
    RUN_TEST(test_setEnd);

    RUN_TEST(test_isActive_withNoStart_withNoEnd);
    RUN_TEST(test_isActive_withNoStart);
    RUN_TEST(test_isActive_withNoEnd);
    RUN_TEST(test_isActive_withEndTimeBeforeStartTime);
    RUN_TEST(test_isActive_withStart_withEnd);

    RUN_TEST(test_isValid_withNoStart_withNoEnd);
    RUN_TEST(test_isValid_withNoStart);
    RUN_TEST(test_isValid_withNoEnd);
    RUN_TEST(test_isValid_withEndTimeBeforeStartTime);
    RUN_TEST(test_isValid_withStart_withEnd);

    RUN_TEST(test_getDuration_withNoStart_withNoEnd);
    RUN_TEST(test_getDuration_withNoStart);
    RUN_TEST(test_getDuration_withNoEnd);
    RUN_TEST(test_getDuration_withEndTimeBeforeStartTime);
    RUN_TEST(test_getDuration_withStart_withEnd);

    RUN_TEST(test_toJsonDocument);

    return UNITY_END();
}

void setup() {
    delay(2000);
    runUnityTests();
}

void loop() {}

#pragma clang diagnostic pop