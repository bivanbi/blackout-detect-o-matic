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

/**
 * Returns the duration between this and another UnixTimeWithMilliSeconds.
 * The sign is positive if this is greater than other, negative if this is less than other.
 *
 * @param other
 * @return Duration between this and other
 */
UnixTimeWithMilliSeconds::Duration UnixTimeWithMilliSeconds::getDuration(UnixTimeWithMilliSeconds other) {
    Duration result;
    UnixTimeWithMilliSeconds greater = *this;
    UnixTimeWithMilliSeconds lesser = other;
    int sign = 1;

    if (lessThan(other)) {
        greater = other;
        lesser = *this;
        sign = -1;
    }

    auto milliSecondDifference = (int) (greater.milliSeconds - lesser.milliSeconds);
    result.sign = sign;

    if (milliSecondDifference < 0) {
        result.seconds = static_cast<unsigned long>(greater.unixTime - lesser.unixTime - 1);
        result.milliSeconds = (unsigned int) 1000 + milliSecondDifference;
    } else {
        result.seconds = static_cast<unsigned long>(greater.unixTime - lesser.unixTime);
        result.milliSeconds = (unsigned int) milliSecondDifference;
    }

    return result;
}

#pragma clang diagnostic pop
