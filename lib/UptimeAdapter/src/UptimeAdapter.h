#ifndef BLACKOUT_DETECT_O_MATIC_UPTIME_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_UPTIME_ADAPTER_H

#include <Arduino.h>

#include "Duration.h"

class UptimeAdapter {
public:
    static unsigned long get();

    static String getFormattedUptime();

    /**
     * 'Pause' uptime uptime for testing purposes.
     * Use it together with set() to for exact
     * uptime value for testing. Subsequent calls to pause
     * will return the same paused uptime.
     *
     * @return uptime in milliseconds at the moment of pausing
     */
    static unsigned long pause();

    /**
     * 'Resume' uptime for testing purposes. It will not jump to
     * the real uptime, rather continue from the (possibly overridden)
     * uptime recorded at the moment of pause() or whatever is explicitly set
     * via set() during pause; maintaining the offset to real uptime.
     */
    static void resume();

    /**
     * 'Override' uptime for testing purposes.
     *
     * @param uptime in milliseconds
     * @return real uptime in milliseconds at the moment issuing set()
     */
    static unsigned long set(unsigned long mockUptime);

    /**
     * Jump back to real uptime cancelling pause() and set() completely.
     */
    static void returnToRealUptime();

private:
    static unsigned long getMockedUptime();

    static bool isPaused;
    static unsigned long pausedAt;

    static bool isOffset;
    static long offset;
};

#endif //BLACKOUT_DETECT_O_MATIC_UPTIME_ADAPTER_H
