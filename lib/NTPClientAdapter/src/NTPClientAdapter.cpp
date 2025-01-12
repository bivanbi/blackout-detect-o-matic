#include <WiFiUdp.h>
#include "NTPClientAdapter.h"
#include "SharedSecrets.h"

NTPClientAdapter::NTPClientAdapter(NTPClient ntpClient) : ntpClient(std::move(ntpClient)) {}

void NTPClientAdapter::begin() {
    ntpClient.begin();
    this->update(); // do an explicit update at the beginning
}

void NTPClientAdapter::update() {
    ntpClient.update();
}

String NTPClientAdapter::getFormattedTime() {
    char buffer [40];
    auto unixTime = (time_t) ntpClient.getEpochTime();
    struct tm * timeInfo = localtime(&unixTime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeInfo);
    return {buffer};
}

WiFiUDP ntpUdp;
NTPClientAdapter ntpClientAdapter = NTPClientAdapter(NTPClient(ntpUdp, NTP_SERVER, NTP_OFFSET, NTP_UPDATE_INTERVAL));
