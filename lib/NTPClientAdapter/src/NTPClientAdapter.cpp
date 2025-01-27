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

void NTPClientAdapter::setServerName(const String &serverName) {
    /**
     * There is something strange going on in WiFiUDP::beginPacket.
     * If we pass the String as string.c_str(), the gethostbyname(host) call
     * in WiFiUDP::beginPacket somehow garbles the host string.
     * That is why we need to copy the string into a char array.
     */
    char *serverNameChar = new char[serverName.length() + 1];
    serverName.toCharArray(serverNameChar, serverName.length() + 1);
    ntpClient.setPoolServerName(serverNameChar);
}

void NTPClientAdapter::setUpdateInterval(unsigned long updateIntervalSeconds) {
    ntpClient.setUpdateInterval(updateIntervalSeconds);
}

void NTPClientAdapter::setTimeOffset(int timeOffsetSeconds) {
    ntpClient.setTimeOffset(timeOffsetSeconds);
}

WiFiUDP ntpUdp;
NTPClientAdapter ntpClientAdapter = NTPClientAdapter(NTPClient(ntpUdp));
