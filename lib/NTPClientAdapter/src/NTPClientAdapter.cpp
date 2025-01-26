#include "NTPClientAdapter.h"

NTPClientAdapter::NTPClientAdapter(NTPClient ntpClient) : ntpClient(std::move(ntpClient)) {}

void NTPClientAdapter::begin() {
    ntpClient.begin();
    this->update(); // do an explicit update at the beginning
}

void NTPClientAdapter::update() {
    ntpClient.update();
}

bool NTPClientAdapter::isTimeSet() const {
    return ntpClient.isTimeSet();
}

UnixTimeWithMilliSeconds NTPClientAdapter::getUnixTime() {
    return UnixTimeWithMilliSeconds(ntpClient.getEpochTime());
}

WiFiUDP ntpUdp;
NTPClientAdapter ntpClientAdapter = NTPClientAdapter(NTPClient(ntpUdp, NTP_SERVER, NTP_OFFSET, NTP_UPDATE_INTERVAL));
