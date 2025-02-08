#ifndef BLACKOUT_DETECT_O_MATIC_TEST_SYSTEM_STATUS_H
#define BLACKOUT_DETECT_O_MATIC_TEST_SYSTEM_STATUS_H

#include <unity.h>
#include <Arduino.h>
#include <WiFiType.h>

#include "SystemStatus.h"

bool statusEquals(SystemStatus::Data status1, SystemStatus::Data status2);

#endif //BLACKOUT_DETECT_O_MATIC_TEST_SYSTEM_STATUS_H
