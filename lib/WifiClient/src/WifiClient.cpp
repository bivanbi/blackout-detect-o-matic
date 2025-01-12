#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#include <WiFi.h>
#include "WifiClient.h"
#include "SharedSecrets.h"

WifiClient::WifiClient() = default;

void WifiClient::connect() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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