#include "RebootTask.h"

void RebootTask::reboot() {
    serialLogger.info("RebootTask: reboot requested");
    executePreRebootTasks();
    serialLogger.info("RebootTask: rebooting...");
    esp_restart();
}

void RebootTask::executePreRebootTasks() {
    serialLogger.info("RebootTask: executePreRebootTasks: executing pre-reboot tasks");
    saveSystemStatus();
    finalDelayBeforeReboot();
}

void RebootTask::saveSystemStatus() {
    serialLogger.info("RebootTask: executePreRebootTasks: saving system status");
    bool result = SystemStatusLoader::save();
    if (result) {
        serialLogger.info("RebootTask: executePreRebootTasks: system status saved");
    } else {
        serialLogger.error("RebootTask: executePreRebootTasks: failed to save system status");
    }
}

void RebootTask::finalDelayBeforeReboot() {
    serialLogger.info("RebootTask: finalDelayBeforeReboot: final delay before reboot");
    delay(1000);
}
