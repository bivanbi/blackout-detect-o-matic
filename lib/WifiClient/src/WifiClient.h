#ifndef BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H
#define BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H

#include <WString.h>
#include <WiFi.h>

class WifiClient {
    public:
        WifiClient();
        void connect(const String& ssid, const String& secret);
        String getMacAddress();
        String getIpAddress();
        bool isConnected();
};

extern WifiClient wifiClient;

#endif //BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H
