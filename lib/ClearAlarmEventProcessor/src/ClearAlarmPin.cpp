#include "ClearAlarmPin.h"

void ClearAlarmPin::init() {
#ifdef CLEAR_ALARM_PIN
    pinMode(CLEAR_ALARM_PIN, INPUT_PULLDOWN);
    attachInterrupt(CLEAR_ALARM_PIN, ClearAlarmPin::onClick, RISING);
#endif
}

void IRAM_ATTR ClearAlarmPin::onClick() {
    clearAlarmEventBuffer.clearAlarmEventCount++;
}
