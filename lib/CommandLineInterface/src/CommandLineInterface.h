#ifndef BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
#define BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H

#include <WString.h>

#include "SystemStatus.h"
#include "RTCAdapter.h"
#include "SerialLogger.h"

class CommandLineInterface {
public:
    CommandLineInterface() = default;

    String executeCommand(String command);

    String getStatus();

    String getHelp();
};

extern CommandLineInterface commandLineInterface;

#endif //BLACKOUT_DETECT_O_MATIC_COMMAND_LINE_INTERFACE_H
