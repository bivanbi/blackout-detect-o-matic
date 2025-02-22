#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "WifiClientAdapter.h"

WifiClientAdapter::WifiClientAdapter() = default;

void WifiClientAdapter::connect(const String& ssid, const String& secret) {
    WiFi.begin(ssid, secret);
}

bool WifiClientAdapter::isConnected() {
    return WiFiClass::status() == WL_CONNECTED;
}

String WifiClientAdapter::getIpAddress() {
    return WiFi.localIP().toString();
}

String WifiClientAdapter::getMacAddress() {
    return WiFi.macAddress();
}

wl_status_t WifiClientAdapter::getStatus() {
    return WiFiClass::status();
}

String WifiClientAdapter::statusToString(wl_status_t status) {
//    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
//    WL_IDLE_STATUS      = 0,
//    WL_NO_SSID_AVAIL    = 1,
//    WL_SCAN_COMPLETED   = 2,
//    WL_CONNECTED        = 3,
//    WL_CONNECT_FAILED   = 4,
//    WL_CONNECTION_LOST  = 5,
//    WL_DISCONNECTED     = 6

    String statusString = "Unknown";
    switch(status) {
        case WL_NO_SHIELD:
            statusString = "No Shield";
            break;
        case WL_IDLE_STATUS:
            statusString = "Idle";
            break;
        case WL_NO_SSID_AVAIL:
            statusString = "No SSID Available";
            break;
        case WL_SCAN_COMPLETED:
            statusString = "Scan Completed";
            break;
        case WL_CONNECTED:
            statusString = "Connected";
            break;
        case WL_CONNECT_FAILED:
            statusString = "Connect Failed";
            break;
        case WL_CONNECTION_LOST:
            statusString = "Connection Lost";
            break;
        case WL_DISCONNECTED:
            statusString = "Disconnected";
            break;
    }
    return statusString + " (" + String(status) + ")";
}

WifiClientAdapter wifiClientAdapter = WifiClientAdapter();

#pragma clang diagnostic pop