#include "AlarmLED.h"

void AlarmLED::init() {
#ifdef ALARM_LED_PIN
    pinMode(ALARM_LED_PIN, OUTPUT);
#endif
}

void AlarmLED::update(bool isAlarmActive) {
#ifdef ALARM_LED_PIN
    digitalWrite(ALARM_LED_PIN, isAlarmActive ? !digitalRead(ALARM_LED_PIN) : LOW);
#endif
}