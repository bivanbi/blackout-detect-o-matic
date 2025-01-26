#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

#include "UnixTimeWithMilliSeconds.h"

UnixTimeWithMilliSeconds::UnixTimeWithMilliSeconds(unsigned long unixTime) {
    this->unixTime = unixTime;
}

UnixTimeWithMilliSeconds::UnixTimeWithMilliSeconds(unsigned long unixTime, unsigned int milliSeconds) {
    this->unixTime = unixTime;
    this->milliSeconds = milliSeconds;
}

UnixTimeWithMilliSeconds::UnixTimeWithMilliSeconds(JsonObject doc) {
    unixTime = doc[UNIX_TIME_FIELD_SECONDS];
    milliSeconds = doc[UNIX_TIME_FIELD_MILLI_SECONDS];
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
    struct tm *timeInfo = gmtime(reinterpret_cast<const time_t *>(&unixTime));
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
Duration UnixTimeWithMilliSeconds::getDuration(UnixTimeWithMilliSeconds other) {
    UnixTimeWithMilliSeconds greater = *this;
    UnixTimeWithMilliSeconds lesser = other;
    int resultSign = 1;
    unsigned long resultSeconds = 0;
    unsigned int resultMilliSeconds = 0;

    if (lessThan(other)) {
        greater = other;
        lesser = *this;
        resultSign = -1;
    }

    auto milliSecondDifference = (int) (greater.milliSeconds - lesser.milliSeconds);

    if (milliSecondDifference < 0) {
        resultSeconds = static_cast<unsigned long>(greater.unixTime - lesser.unixTime - 1);
        resultMilliSeconds = (unsigned int) 1000 + milliSecondDifference;
    } else {
        resultSeconds = static_cast<unsigned long>(greater.unixTime - lesser.unixTime);
        resultMilliSeconds = (unsigned int) milliSecondDifference;
    }

    return {resultSign, resultSeconds, resultMilliSeconds};
}

JsonDocument UnixTimeWithMilliSeconds::toJsonDocument() {
    JsonDocument doc;
    doc[UNIX_TIME_FIELD_SECONDS] = unixTime;
    doc[UNIX_TIME_FIELD_MILLI_SECONDS] = milliSeconds;
    return doc;
}

#pragma clang diagnostic pop
