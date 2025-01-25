#ifndef BLACKOUT_DETECT_O_MATIC_DURATION_H
#define BLACKOUT_DETECT_O_MATIC_DURATION_H

class Duration {
public:
    int getSign();

    unsigned long getSeconds();

    unsigned int getMilliSeconds();

    Duration(int sign, unsigned long seconds, unsigned int milliSeconds);

    bool greaterThan(Duration duration);

    bool lessThan(Duration duration);

private:
    int sign;
    unsigned long seconds;
    unsigned int milliSeconds;
};

#endif //BLACKOUT_DETECT_O_MATIC_DURATION_H
