#ifndef BLACKOUT_DETECT_O_MATIC_RESET_SYSTEM_STATUS_PIN_H
#define BLACKOUT_DETECT_O_MATIC_RESET_SYSTEM_STATUS_PIN_H

#include <Arduino.h>

#include "Configuration.h"
#include "ResetSystemStatusEventProcessor.h"

class ResetSystemStatusPin {
public:
    static void init();

    static void onClick();
};

#endif //BLACKOUT_DETECT_O_MATIC_RESET_SYSTEM_STATUS_PIN_H
