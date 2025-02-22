#include "UptimeAdapter.h"

unsigned long UptimeAdapter::get() {
    if (isPaused || isOffset) {
        return getMockedUptime();
    }

    return millis();
}

unsigned long UptimeAdapter::pause() {
    if (isPaused) {
        return pausedAt;
    }

    pausedAt = millis();
    isPaused = true;
    return pausedAt;
}

void UptimeAdapter::resume() {
    if (!isPaused) {
        return;
    }

    isPaused = false;
    isOffset = true;
    offset = (long) (pausedAt - millis());
}

unsigned long UptimeAdapter::set(unsigned long mockUptime) {
    unsigned long realUptime = millis();
    offset = (long) (mockUptime - realUptime);
    isOffset = true;
    if (isPaused) {
        pausedAt = mockUptime;
    }
    return realUptime;
}

void UptimeAdapter::returnToRealUptime() {
    isOffset = false;
    isPaused = false;
}

unsigned long UptimeAdapter::getMockedUptime() {
    if (isPaused) {
        return pausedAt;
    }

    return millis() + offset;
}

String UptimeAdapter::getFormattedUptime() {
    unsigned long uptime = get();
    return Duration::getFormattedDuration(uptime);
}

bool UptimeAdapter::isPaused = false;
unsigned long UptimeAdapter::pausedAt = 0;
bool UptimeAdapter::isOffset = false;
long UptimeAdapter::offset = 0;