#ifndef BLACKOUT_DETECT_O_MATIC_PERIODIC_TASK_SCHEDULER_H
#define BLACKOUT_DETECT_O_MATIC_PERIODIC_TASK_SCHEDULER_H

#include <Arduino.h>

#include "RebootTask.h"
#include "Configuration.h"
#include "Logger.h"
#include "SystemStatus.h"
#include "WifiClientAdapter.h"
#include "RTCAdapter.h"
#include "NTPClientAdapter.h"
#include "PowerDetectorPin.h"
#include "PowerChangeEventProcessor.h"
#include "ResetSystemStatusEventProcessor.h"
#include "ClearAlarmEventProcessor.h"
#include "AlarmLED.h"
#include "SystemStatusLoader.h"
#include "UptimeAdapter.h"
#include "SyncTimeTask.h"
#include "LogRotate.h"

class PeriodicTaskScheduler {
    /**
     * Ticks in milliseconds for periodic tasks.
     */
    struct TickCounter {
        unsigned long timeSync = 0;
        unsigned long serialHeartBeat = 0;
        unsigned long fileHeartBeat = 0;
        unsigned long logRotate = 0;
        unsigned long saveSystemStatus = 0;
        unsigned long reboot = 0;
    };

public:
    PeriodicTaskScheduler() = default;

    static void init();

    /**
     * Reboot with optional delay.
     * @param delay in seconds. Set to 0 to reboot immediately.
     */
    static void scheduleReboot(long delay = 0);

    /**
     * Timer interrupt handler - indicate, that periodic tasks are due.
     */
    static void onTimerInterrupt();

    /**
     * Execute time consuming periodic tasks.
     */
    static void loop();

    static void syncTime(bool force = false);

private:
    static void tick();

    static void heartBeat();

    static bool isTaskDue(unsigned long taskRunInterval);

    static void updateSystemStatus();

    static void processEvents();

    static void reboot();

    static void rotateLogs();

    static void saveSystemStatus();

    static String getHeartBeatMessage();

    static hw_timer_t *periodicTaskTimer;
    static bool periodicTasksAreDue;
    static TickCounter tickCounter;
    static long rebootRequested;
};

#endif //BLACKOUT_DETECT_O_MATIC_PERIODIC_TASK_SCHEDULER_H
