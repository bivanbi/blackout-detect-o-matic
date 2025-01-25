#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

#include "UnixTimeWithMilliSeconds.h"

UnixTimeWithMilliSeconds::UnixTimeWithMilliSeconds(time_t unixTime, unsigned int milliSeconds) {
    this->unixTime = unixTime;
    this->milliSeconds = milliSeconds;
}

unsigned long UnixTimeWithMilliSeconds::getUnixTime() {
    return unixTime;
}

unsigned int UnixTimeWithMilliSeconds::getMilliSeconds() {
    return milliSeconds;
}

String UnixTimeWithMilliSeconds::getMillisAsZeroPaddedString() {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << getMilliSeconds();
    std::string result = ss.str();
    return result.c_str();
}

String UnixTimeWithMilliSeconds::getFormattedTime(const String &format) {
    char buffer[40];
    struct tm *timeInfo = gmtime(&unixTime);
    strftime(buffer, 80, format.c_str(), timeInfo);
    return {buffer};
}

bool UnixTimeWithMilliSeconds::equals(UnixTimeWithMilliSeconds other) {
    return unixTime == other.unixTime && milliSeconds == other.milliSeconds;
}

bool UnixTimeWithMilliSeconds::greaterThan(UnixTimeWithMilliSeconds other) {
    return unixTime > other.unixTime || (unixTime == other.unixTime && milliSeconds > other.milliSeconds);
}

bool UnixTimeWithMilliSeconds::lessThan(UnixTimeWithMilliSeconds other) {
    return unixTime < other.unixTime || (unixTime == other.unixTime && milliSeconds < other.milliSeconds);
}

bool UnixTimeWithMilliSeconds::greaterOrEqual(UnixTimeWithMilliSeconds other) {
    return greaterThan(other) || equals(other);
}

#pragma clang diagnostic pop
