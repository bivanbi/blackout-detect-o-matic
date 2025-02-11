#ifndef BLACKOUT_DETECT_O_MATIC_RTC_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_RTC_ADAPTER_H

#include <ESP32Time.h>
#include "UnixTimeWithMilliSeconds.h"

class RTCAdapter {
public:
    enum ClockSource {
        NONE,
        NTP,
        FILE_DATE,
    };

    explicit RTCAdapter(int offset = 0);

    bool isTimeSet();

    void setTime(ClockSource source, UnixTimeWithMilliSeconds unixTime);

    UnixTimeWithMilliSeconds getTime();

    unsigned long getMicros();

    int getOffset() const;

    ClockSource getClockSource();

    String clockSourceToString(ClockSource source);

    int getID() const {
        return id;
    }

    /**
     * Pause the RTC clock for testing purposes.
     * Use it together with set() to for exact
     * time value for testing. Subsequent calls to pause
     * will return the same paused time.
     *
     * @return time when the clock was paused
     */
    UnixTimeWithMilliSeconds pause();

    /**
     * Jump back to real uptime cancelling pause() and set() completely.
     */
    void returnToRealRtcTime();

protected:
    static int ID;

private:
    int id;
    int offset;
    ESP32Time rtc;
    ClockSource clockSource;

    bool isPaused = false;
    UnixTimeWithMilliSeconds pausedAt;
};

extern RTCAdapter rtcAdapter;

#endif //BLACKOUT_DETECT_O_MATIC_RTC_ADAPTER_H
