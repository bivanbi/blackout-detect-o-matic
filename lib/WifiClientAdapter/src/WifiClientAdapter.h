#ifndef BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H
#define BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H

#include <WString.h>
#include <WiFi.h>

class WifiClientAdapter {
    public:
        WifiClientAdapter();
        void connect(const String& ssid, const String& secret);
        String getMacAddress();
        String getIpAddress();
        bool isConnected();
        String statusToString(wl_status_t status);
        wl_status_t getStatus();
};

extern WifiClientAdapter wifiClientAdapter;

#endif //BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H
