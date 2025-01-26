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

    int getID() const {
        return id;
    }

protected:
    static int ID;

private:
    int id;
    int offset;
    ESP32Time rtc;
    ClockSource clockSource;
};

extern RTCAdapter rtcAdapter;

#endif //BLACKOUT_DETECT_O_MATIC_RTC_ADAPTER_H
