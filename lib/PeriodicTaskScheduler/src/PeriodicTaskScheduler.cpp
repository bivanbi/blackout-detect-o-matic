#include "PeriodicTaskScheduler.h"

void PeriodicTaskScheduler::init() {
    periodicTaskTimer = timerBegin(0, 80, true); // set timer frequency to 1 MHz
    timerAttachInterrupt(periodicTaskTimer, &onTimerInterrupt, true);
    timerAlarmWrite(periodicTaskTimer, PERIODIC_TASK_INTERVAL * 1000, true); // in microseconds
    timerAlarmEnable(periodicTaskTimer);
}

void IRAM_ATTR PeriodicTaskScheduler::onTimerInterrupt() {
    periodicTasksAreDue = true;
    timerAlarmDisable(periodicTaskTimer);
}

void PeriodicTaskScheduler::loop() {
    if (periodicTasksAreDue) {
        tick();
        reboot();
        syncTime();
        rotateLogs();
        processEvents();
        updateSystemStatus();
        AlarmLED::update(systemStatus.isAlarmActive());
        saveSystemStatus();
        heartBeat();
        periodicTasksAreDue = false;
    }
}

void PeriodicTaskScheduler::tick() {
    tickCounter.timeSync += PERIODIC_TASK_INTERVAL;
    tickCounter.serialHeartBeat += PERIODIC_TASK_INTERVAL;
    tickCounter.fileHeartBeat += PERIODIC_TASK_INTERVAL;
    tickCounter.logRotate += PERIODIC_TASK_INTERVAL;

    if (rebootRequested >= 0) {
        tickCounter.reboot += PERIODIC_TASK_INTERVAL;
    }
}

void PeriodicTaskScheduler::reboot() {
    if (rebootRequested >= 0 && tickCounter.reboot >= rebootRequested * 1000) {
        RebootTask::reboot();
    }
}

void PeriodicTaskScheduler::syncTime(bool force) {
    if (isSyncTimeDue() || force) {
        SyncTimeTask::syncTime();
        tickCounter.timeSync = 0;
    }
}

bool PeriodicTaskScheduler::isSyncTimeDue() {
    return tickCounter.timeSync >= configuration.getNtpUpdateInterval() * 1000;
}

void PeriodicTaskScheduler::processEvents() {
    powerChangeEventProcessor.processEvents(PowerDetectorPin::isPowerOn());
    clearAlarmEventProcessor.processEvents();
    resetSystemStatusEventProcessor.processEvents();
}

void PeriodicTaskScheduler::updateSystemStatus() {
    systemStatus.setWifiStatus(wifiClientAdapter.getStatus());
    systemStatus.setClockStatus(ntpClientAdapter.isTimeSet());
}

void PeriodicTaskScheduler::saveSystemStatus() {
    if (isSystemStatusSaveDue()) {
        SystemStatusLoader::save();
        tickCounter.saveSystemStatus = 0;
    }
}

void PeriodicTaskScheduler::rotateLogs() {
    if (isLogRotateDue()) {
        LogRotate::rotate();
        tickCounter.logRotate = 0;
    }
}

bool PeriodicTaskScheduler::isSystemStatusSaveDue() {
    return tickCounter.saveSystemStatus >= configuration.getSystemStatusSaveInterval() * 1000;
}

void PeriodicTaskScheduler::heartBeat() {
    if (isSerialHeartBeatDue()) {
        Logger::info(getHeartBeatMessage(), {Logger::Channel::SERIAL_PORT});
        tickCounter.serialHeartBeat = 0;
    }

    if (isFileHeartBeatDue()) {
        Logger::info(getHeartBeatMessage(), {Logger::Channel::PERSISTENT_STORAGE});
        tickCounter.fileHeartBeat = 0;
    }
}

bool PeriodicTaskScheduler::isSerialHeartBeatDue() {
    return tickCounter.serialHeartBeat >= configuration.getHeartbeatSerialLogInterval() * 1000;
}

bool PeriodicTaskScheduler::isFileHeartBeatDue() {
    return tickCounter.fileHeartBeat >= configuration.getHeartbeatFileLogInterval() * 1000;
}

bool PeriodicTaskScheduler::isLogRotateDue() {
    return tickCounter.logRotate >= configuration.getLogRotateTaskInterval() * 1000;
}

void PeriodicTaskScheduler::scheduleReboot(long delay) {
    rebootRequested = delay;
    Logger::info("PeriodicTaskScheduler: Reboot scheduled in " + String(delay) + " seconds");
}

String PeriodicTaskScheduler::getHeartBeatMessage() {
    return "uptime: " + UptimeAdapter::getFormattedUptime() + ", WiFi status: " +
           wifiClientAdapter.statusToString(wifiClientAdapter.getStatus()) + ", clock set: " + ntpClientAdapter.isTimeSet()
           + ", blackout count: " + systemStatus.getBlackoutCount() + ", reboot count: " + systemStatus.getRebootCount()
           + ", alarm active: " + systemStatus.isAlarmActive()
           + ", pending power event counts: " + powerChangeEventBuffer.powerDownEventCount + " down, "
           + powerChangeEventBuffer.powerUpEventCount + " up"
           + ", Power state: " + (PowerDetectorPin::isPowerOn() ? "on" : "off")
           + ", pending clear alarm event count: " + clearAlarmEventBuffer.clearAlarmEventCount
           + ", pending reset status event count: " + resetSystemStatusEventBuffer.resetEventCount
           + ", clear alarm port status: " + digitalRead(CLEAR_ALARM_PIN);
}

hw_timer_t *PeriodicTaskScheduler::periodicTaskTimer = nullptr;
bool PeriodicTaskScheduler::periodicTasksAreDue = false;
PeriodicTaskScheduler::TickCounter PeriodicTaskScheduler::tickCounter;
long PeriodicTaskScheduler::rebootRequested = -1; // Reboot delayed by given seconds. Set to 0 to reboot immediately.