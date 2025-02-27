#ifndef BLACKOUT_DETECT_O_MATIC_TELNET_SERVER_H
#define BLACKOUT_DETECT_O_MATIC_TELNET_SERVER_H

#include <Arduino.h>
#include <ESPTelnet.h>
#include "CommandLineInterface.h"
#include "Logger.h"
#include "SystemStatusLoader.h"

class TelnetServer {
public:
    TelnetServer() = default;

    static void begin(unsigned int port);

    static void loop();

private:
    static void onConnect(String ip);

    static void onConnectionAttempt(String ip);

    static void onReconnect(String ip);

    static void onDisconnect(String ip);

    static void onInputReceived(String input);

    static void requireAuthentication();

    static String authenticate(String input);

    static String getPasswordPrompt();

    static String getHelp();

    static bool clientAuthenticated;
    static const String prompt;
    static const String logTag;
};

#endif //BLACKOUT_DETECT_O_MATIC_TELNET_SERVER_H
