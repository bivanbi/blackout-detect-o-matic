#ifndef BLACKOUT_DETECT_O_MATIC_RTC_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_RTC_ADAPTER_H

#include <ESP32Time.h>
#include "UnixTimeWithMilliSeconds.h"

#define RTC_OFFSET 3600

class RTCAdapter {
public:
    enum ClockSource {
        NONE,
        NTP,
        FILE_DATE,
    };

    explicit RTCAdapter(int offset = 0);

    bool isTimeSet();

    void setTime(ClockSource source, unsigned long unixTime, int millis = 0);

    void setTime(ClockSource source, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute,
                 uint8_t second);

    UnixTimeWithMilliSeconds getTime();

    time_t getUnixTime();

    unsigned long getMillis();

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
