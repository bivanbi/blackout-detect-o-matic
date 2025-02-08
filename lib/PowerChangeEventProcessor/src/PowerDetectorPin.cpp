#include "PowerDetectorPin.h"

void PowerDetectorPin::init() {
    pinMode(POWER_DETECTOR_PIN, INPUT_PULLDOWN);
    attachInterrupt(POWER_DETECTOR_PIN, PowerDetectorPin::onChange, CHANGE);
}

void IRAM_ATTR PowerDetectorPin::onChange() {
    powerChangeEventBuffer.powerDownEventCount++;
    // do not call isPowerOn here to conserve time and RAM
    if (HIGH == digitalRead(POWER_DETECTOR_PIN)) {
        powerChangeEventBuffer.powerUpEventCount++;
    } else {
        powerChangeEventBuffer.powerDownEventCount++;
    }
}

bool PowerDetectorPin::isPowerOn() {
    return HIGH == digitalRead(POWER_DETECTOR_PIN);
}
