#ifndef BLACKOUT_DETECT_O_MATIC_REBOOT_TASK_H
#define BLACKOUT_DETECT_O_MATIC_REBOOT_TASK_H

#include <Arduino.h>

#include "Logger.h"
#include "SystemStatusLoader.h"

class RebootTask {
public:
    static void reboot();

private:
    static void executePreRebootTasks();

    static void saveSystemStatus();

    static void unmount();

    static void finalDelayBeforeReboot();
};

#endif //BLACKOUT_DETECT_O_MATIC_REBOOT_TASK_H
