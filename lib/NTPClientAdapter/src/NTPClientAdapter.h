#include <optional>
#include <NTPClient.h>
#include <WiFiUdp.h>

#ifndef BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H
#define BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H

class NTPClientAdapter {
    public:
        explicit NTPClientAdapter(NTPClient ntpClient);
        void begin();
        void update();
        String getFormattedTime();
    private:
        NTPClient ntpClient;
};

extern NTPClientAdapter ntpClientAdapter;

#endif //BLACKOUT_DETECT_O_MATIC_NTP_CLIENT_ADAPTER_H
