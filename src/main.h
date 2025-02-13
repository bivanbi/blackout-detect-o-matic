#ifndef BLACKOUT_DETECT_O_MATIC_MAIN_H
#define BLACKOUT_DETECT_O_MATIC_MAIN_H

#include <Arduino.h>
#include <random>

#include "Configuration.h"
#include "SerialLogger.h"
#include "WifiClientAdapter.h"
#include "NTPClientAdapter.h"
#include "RTCAdapter.h"
#include "SystemStatus.h"
#include "PowerDetectorPin.h"
#include "PowerChangeEventProcessor.h"
#include "ClearAlarmPin.h"
#include "ClearAlarmEventProcessor.h"
#include "ResetSystemStatusPin.h"
#include "ResetSystemStatusEventProcessor.h"
#include "AlarmLED.h"
#include "TelnetServer.h"
#include "PeriodicTaskScheduler.h"
#include "SystemStatusLoader.h"
#include "ConfigurationLoader.h"
#include "SerialCommandInterface.h"

#define LOG_FILEPATH_FORMAT LOG_DIRECTORY + "/log_%Y-%m-%d.log";
#define SYSTEM_STATUS_FILEPATH "/system_status.json"

void initConfiguration();

void initPersistentStorage();

void initWiFi();

void initNTP();

void initPins();

void initTelnetServer();

#endif //BLACKOUT_DETECT_O_MATIC_MAIN_H
