#ifndef BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H

#include <optional>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "NTPClientAdapter.h"
#include "SharedSecrets.h"
#include "UnixTimeWithMilliSeconds.h"

class NTPClientAdapter {
public:
    explicit NTPClientAdapter(NTPClient ntpClient);

    void begin();

    void update();

    UnixTimeWithMilliSeconds getUnixTime();

    bool isTimeSet() const;

private:
    NTPClient ntpClient;
};

extern NTPClientAdapter ntpClientAdapter;

#endif //BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H
