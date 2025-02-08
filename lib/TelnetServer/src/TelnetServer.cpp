#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"

#include "TelnetServer.h"

ESPTelnet telnet;
String prompt = "> ";

void TelnetServer::begin(unsigned int port) {
    telnet.onConnect(onConnect);
    telnet.onConnectionAttempt(onConnectionAttempt);
    telnet.onReconnect(onReconnect);
    telnet.onDisconnect(onDisconnect);
    telnet.onInputReceived(onInputReceived);

    Serial.print("- Telnet: ");
    if (telnet.begin(port, false)) {
        serialLogger.info("TelnetServer: listening on " + String(WiFi.localIP()) + ":" + port);
    } else {
        serialLogger.error("TelnetServer: failed to start");
    }
}

void TelnetServer::loop() {
    telnet.loop();
}

void TelnetServer::onConnect(String ip) {
    serialLogger.info("TelnetServer: onConnect: client ip: " + ip);
    telnet.println(TelnetServer::getWelcomeMessage());
    telnet.print(prompt);
}

void TelnetServer::onConnectionAttempt(String ip) {
    serialLogger.info("TelnetServer: onConnectionAttempt: client ip: " + ip);
}

void TelnetServer::onReconnect(String ip) {
    serialLogger.info("TelnetServer: onReconnect: client ip: " + ip);
}

void TelnetServer::onDisconnect(String ip) {
    serialLogger.info("TelnetServer: onDisconnect: client ip: " + ip);
}

void TelnetServer::onInputReceived(String input) {
    serialLogger.debug("TelnetServer: onInputReceived: " + input);

    if (input.equals("exit") || input.equals("quit")) {
        telnet.disconnectClient();
    } else if (input.equals("help")) {
        telnet.println(TelnetServer::getHelp());
        telnet.print(prompt);
    } else if (input.equals("reboot")) {
        reboot();
    } else {
        telnet.println(commandLineInterface.executeCommand(input));
        telnet.print(prompt);
    }
}

String TelnetServer::getHelp() {
    return "quit - disconnect\n"
           + commandLineInterface.getHelp()
           + "reboot - reboot the device\n";
}

String TelnetServer::getWelcomeMessage() {
    return {"Welcome to Blackout Detect-o-matic\n"
            "Type 'help' for a list of commands\n"};
}

void TelnetServer::reboot() {
    telnet.println("Rebooting, bye!");
    telnet.disconnectClient(true);
    esp_restart();
}

#pragma clang diagnostic pop