#include "RebootTask.h"

void RebootTask::reboot() {
    Logger::info("RebootTask: reboot requested");
    executePreRebootTasks();
    Logger::info("RebootTask: rebooting...");
    esp_restart();
}

void RebootTask::executePreRebootTasks() {
    Logger::info("RebootTask: executePreRebootTasks: executing pre-reboot tasks");
    saveSystemStatus();
    unmount();
    finalDelayBeforeReboot();
}

void RebootTask::saveSystemStatus() {
    Logger::info("RebootTask: executePreRebootTasks: saving system status");
    bool result = SystemStatusLoader::save();
    if (result) {
        Logger::info("RebootTask: executePreRebootTasks: system status saved");
    } else {
        Logger::error("RebootTask: executePreRebootTasks: failed to save system status");
    }
}

void RebootTask::unmount() {
    Logger::info("RebootTask: unmount: unmounting filesystem");
    persistentStorage.unMount();
}

void RebootTask::finalDelayBeforeReboot() {
    Logger::info("RebootTask: finalDelayBeforeReboot: final delay before reboot");
    delay(1000);
}
