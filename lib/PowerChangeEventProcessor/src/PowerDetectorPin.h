#ifndef BLACKOUT_DETECT_O_MATIC_POWER_DETECTOR_PIN_H
#define BLACKOUT_DETECT_O_MATIC_POWER_DETECTOR_PIN_H

#include <Arduino.h>
#include "Configuration.h"
#include "PowerChangeEventProcessor.h"

class PowerDetectorPin {
public:
    static void init();

    static void onChange();

    static bool isPowerOn();
};


#endif //BLACKOUT_DETECT_O_MATIC_POWER_DETECTOR_PIN_H
