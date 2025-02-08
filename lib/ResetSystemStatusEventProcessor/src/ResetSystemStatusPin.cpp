#include "ResetSystemStatusPin.h"

void ResetSystemStatusPin::init() {
#ifdef RESET_SYSTEM_STATUS_PIN
    pinMode(RESET_SYSTEM_STATUS_PIN, INPUT_PULLDOWN);
    attachInterrupt(RESET_SYSTEM_STATUS_PIN, ResetSystemStatusPin::onClick, RISING);
#endif
}

void IRAM_ATTR ResetSystemStatusPin::onClick() {
    resetSystemStatusEventBuffer.resetEventCount++;
}
