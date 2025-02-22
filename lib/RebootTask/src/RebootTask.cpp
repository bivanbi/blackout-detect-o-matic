#include "RebootTask.h"

const String RebootTask::logTag = "RebootTask: ";

void RebootTask::reboot() {
    Logger::info(logTag + "reboot requested");
    executePreRebootTasks();
    Logger::info(logTag + "rebooting...");
    esp_restart();
}

void RebootTask::executePreRebootTasks() {
    Logger::info(logTag + "executePreRebootTasks: executing pre-reboot tasks");
    saveSystemStatus();
    unmount();
    finalDelayBeforeReboot();
}

void RebootTask::saveSystemStatus() {
    Logger::info(logTag + "executePreRebootTasks: saving system status");
    bool result = SystemStatusLoader::save();
    if (result) {
        Logger::info(logTag + "executePreRebootTasks: system status saved");
    } else {
        Logger::error(logTag + "executePreRebootTasks: failed to save system status");
    }
}

void RebootTask::unmount() {
    Logger::info(logTag + "unmount: unmounting filesystem");
    persistentStorage.unMount();
}

void RebootTask::finalDelayBeforeReboot() {
    Logger::info(logTag + "finalDelayBeforeReboot: final delay before reboot");
    delay(1000);
}
