#ifndef BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H

#include <optional>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "NTPClientAdapter.h"
#include "UnixTimeWithMilliSeconds.h"

class NTPClientAdapter {
public:
    explicit NTPClientAdapter(NTPClient ntpClient);

    void begin();

    void update();

    UnixTimeWithMilliSeconds getUnixTime();

    bool isTimeSet() const;

    void setServerName(const String &serverName);

    void setUpdateInterval(unsigned long updateIntervalSeconds);

    void setTimeOffset(int timeOffsetSeconds);

private:
    NTPClient ntpClient;
};

extern WiFiUDP ntpUdp;
extern NTPClientAdapter ntpClientAdapter;

#endif //BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H
