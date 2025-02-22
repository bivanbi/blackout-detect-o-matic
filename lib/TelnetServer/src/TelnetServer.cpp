#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"

#include "TelnetServer.h"

ESPTelnet telnet;
const String TelnetServer::prompt = "> ";
const String TelnetServer::logTag = "TelnetServer: ";

void TelnetServer::begin(unsigned int port) {
    telnet.onConnect(onConnect);
    telnet.onConnectionAttempt(onConnectionAttempt);
    telnet.onReconnect(onReconnect);
    telnet.onDisconnect(onDisconnect);
    telnet.onInputReceived(onInputReceived);

    if (telnet.begin(port, true)) {
        Logger::info(logTag + "listening on " + String(WiFi.localIP()) + ":" + port);
    } else {
        Logger::error(logTag + "failed to start");
    }
}

void TelnetServer::loop() {
    telnet.loop();
}

void TelnetServer::onConnect(String ip) {
    Logger::info(logTag + "onConnect: client ip: " + ip);
    requireAuthentication();
}

void TelnetServer::onConnectionAttempt(String ip) {
    Logger::info(logTag + "onConnectionAttempt: client ip: " + ip);
}

void TelnetServer::onReconnect(String ip) {
    Logger::info(logTag + "onReconnect: client ip: " + ip);
    requireAuthentication();
}

void TelnetServer::onDisconnect(String ip) {
    clientAuthenticated = false;
    Logger::info(logTag + "onDisconnect: client ip: " + ip);
}

void TelnetServer::onInputReceived(String input) {
    Logger::debug(logTag + "onInputReceived: " + input);

    if (clientAuthenticated) {
        if (input.equals("exit") || input.equals("quit")) {
            telnet.disconnectClient();
        } else if (input.equals("help")) {
            telnet.print("quit - disconnect\n" + commandLineInterface.help() + prompt);
        } else {
            telnet.print(commandLineInterface.executeCommand(input) + prompt);
        }
    } else {
        telnet.print(authenticate(input));
    }
}

void TelnetServer::requireAuthentication() {
    clientAuthenticated = false;
    telnet.print(TelnetServer::getPasswordPrompt());
}

String TelnetServer::authenticate(String input) {
    if (input.equals(configuration.getTelnetPassword())) {
        clientAuthenticated = true;
        return {"Welcome to Blackout Detect-o-matic\n"
                "Type 'help' for a list of commands\n" + prompt};
    }

    Logger::info(logTag + "authentication failed");
    return {"Authentication failed\n" + getPasswordPrompt()};
}

String TelnetServer::getPasswordPrompt() {
    return "Enter password: ";
}

String TelnetServer::getHelp() {
    return "quit - disconnect\n"
           + commandLineInterface.help();
}

bool TelnetServer::clientAuthenticated = false;

#pragma clang diagnostic pop