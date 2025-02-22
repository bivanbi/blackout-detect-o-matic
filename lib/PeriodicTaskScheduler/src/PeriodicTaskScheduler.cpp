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
    if (isTaskDue(tickCounter.timeSync) || force) {
        SyncTimeTask::syncTime();
        tickCounter.timeSync = 0;
    }
}

bool PeriodicTaskScheduler::isTaskDue(unsigned long taskRunInterval) {
    return taskRunInterval >= configuration.getNtpUpdateInterval() * 1000;
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
    if (isTaskDue(tickCounter.saveSystemStatus)) {
        SystemStatusLoader::save();
        tickCounter.saveSystemStatus = 0;
    }
}

void PeriodicTaskScheduler::rotateLogs() {
    if (isTaskDue(tickCounter.logRotate)) {
        LogRotate::rotate();
        tickCounter.logRotate = 0;
    }
}

void PeriodicTaskScheduler::heartBeat() {
    if (isTaskDue(tickCounter.serialHeartBeat)) {
        Logger::info(getHeartBeatMessage(), {Logger::Channel::SERIAL_PORT});
        tickCounter.serialHeartBeat = 0;
    }

    if (isTaskDue(tickCounter.fileHeartBeat)) {
        Logger::info(getHeartBeatMessage(), {Logger::Channel::PERSISTENT_STORAGE});
        tickCounter.fileHeartBeat = 0;
    }
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