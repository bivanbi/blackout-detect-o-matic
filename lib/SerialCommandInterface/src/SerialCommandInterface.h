#ifndef BLACKOUT_DETECT_O_MATIC_SERIAL_COMMAND_INTERFACE_H
#define BLACKOUT_DETECT_O_MATIC_SERIAL_COMMAND_INTERFACE_H

#include <Arduino.h>
#include "Logger.h"
#include "CommandLineInterface.h"

class SerialCommandInterface {
public:
    SerialCommandInterface() = default;

    void setup();

    void loop();

private:
    void readSerial();

    void executeCommand();

    String inputBuffer;
    bool commandReceived = false;
    static const String logTag;
};

extern SerialCommandInterface serialCLI;

#endif //BLACKOUT_DETECT_O_MATIC_SERIAL_COMMAND_INTERFACE_H
