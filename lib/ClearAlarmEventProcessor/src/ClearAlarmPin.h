#ifndef BLACKOUT_DETECT_O_MATIC_CLEAR_ALARM_PIN_H
#define BLACKOUT_DETECT_O_MATIC_CLEAR_ALARM_PIN_H

#include <Arduino.h>

#include "Configuration.h"
#include "ClearAlarmEventProcessor.h"

class ClearAlarmPin {
public:
    static void init();

    static void onClick();
};

#endif //BLACKOUT_DETECT_O_MATIC_CLEAR_ALARM_PIN_H
