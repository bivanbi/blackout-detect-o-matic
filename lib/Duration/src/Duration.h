#ifndef BLACKOUT_DETECT_O_MATIC_DURATION_H
#define BLACKOUT_DETECT_O_MATIC_DURATION_H

#include <cstdio>
#include <WString.h>

class Duration {
public:
    int getSign();

    unsigned long getSeconds();

    unsigned int getMilliSeconds();

    Duration(int sign, unsigned long seconds, unsigned int milliSeconds);

    bool greaterThan(Duration other);

    bool lessThan(Duration other);

    String getFormattedDuration();

    static String getFormattedDuration(unsigned long milliSeconds);

    static String getFormattedDuration(unsigned long seconds, unsigned int milliSeconds);

private:
    int sign;
    unsigned long seconds;
    unsigned int milliSeconds;
};

#endif //BLACKOUT_DETECT_O_MATIC_DURATION_H
