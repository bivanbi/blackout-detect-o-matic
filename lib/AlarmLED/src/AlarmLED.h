#ifndef BLACKOUT_DETECT_O_MATIC_ALARM_LED_H
#define BLACKOUT_DETECT_O_MATIC_ALARM_LED_H

#include <Arduino.h>

#include "Configuration.h"

class AlarmLED {
public:
    static void init();

    static void update(bool isAlarmActive);
};

#endif //BLACKOUT_DETECT_O_MATIC_ALARM_LED_H
