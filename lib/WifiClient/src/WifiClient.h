#ifndef BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H
#define BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H

class WifiClient {
    public:
        WifiClient();
        void connect();
        String getMacAddress();
        String getIpAddress();
        bool isConnected();
};

extern WifiClient wifiClient;

#endif //BLACKOUT_DETECT_O_MATIC_WIFI_CLIENT_H
