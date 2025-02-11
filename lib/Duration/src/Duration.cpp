#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

#include "Duration.h"

Duration::Duration(int sign, unsigned long seconds, unsigned int milliSeconds) {
    this->sign = sign;
    this->seconds = seconds;
    this->milliSeconds = milliSeconds;
}

int Duration::getSign() {
    return sign;
}

unsigned long Duration::getSeconds() {
    return seconds;
}

unsigned int Duration::getMilliSeconds() {
    return milliSeconds;
}

bool Duration::greaterThan(Duration other) {
    if (seconds > other.getSeconds()) {
        return true;
    } else if (seconds < other.getSeconds()) {
        return false;
    }

    return milliSeconds > other.getMilliSeconds();
}

bool Duration::lessThan(Duration other) {
    if (seconds < other.getSeconds()) {
        return true;
    } else if (seconds > other.getSeconds()) {
        return false;
    }

    return milliSeconds < other.getMilliSeconds();
}

String Duration::getFormattedDuration() {
    return getFormattedDuration(seconds, milliSeconds);
}

String Duration::getFormattedDuration(unsigned long milliSeconds) {
    return getFormattedDuration(milliSeconds / 1000, milliSeconds % 1000);
}

String Duration::getFormattedDuration(unsigned long seconds, unsigned int milliSeconds) {
    unsigned int resultDays = seconds / 86400;
    unsigned int resultHours = (seconds % 86400) / 3600;
    unsigned int resultMinutes = (seconds % 3600) / 60;
    unsigned int resultSeconds = seconds % 60;

    char result[80];
    sprintf(result, "%d days %02d:%02d:%02d.%03d", resultDays, resultHours, resultMinutes, resultSeconds, milliSeconds);
    return {result};
}

#pragma clang diagnostic pop