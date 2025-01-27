#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "WifiClient.h"

WifiClient::WifiClient() = default;

void WifiClient::connect(const String& ssid, const String& secret) {
    WiFi.begin(ssid, secret);
}

bool WifiClient::isConnected() {
    return WiFiClass::status() == WL_CONNECTED;
}

String WifiClient::getIpAddress() {
    return WiFi.localIP().toString();
}

String WifiClient::getMacAddress() {
    return WiFi.macAddress();
}

WifiClient wifiClient = WifiClient();
#pragma clang diagnostic pop