#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#include "SerialCommandInterface.h"

void SerialCommandInterface::setup() {
    Serial.begin(115200);
    serialLogger.info("Serial Command Interface: ready to receive commands");
}

void SerialCommandInterface::loop() {
    readSerial();
    executeCommand();
}

void SerialCommandInterface::readSerial() {
    if (Serial.available() > 0) {
        char input = Serial.read();
        if (input == '\b') {
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);
                Serial.print("\b \b");
            }
        } else
        if (input == '\n') {
            commandReceived = true;
            serialLogger.debug("SerialCommandInterface::readSerial: Received command: '" + inputBuffer + "'");
        } else {
            inputBuffer += input;
            Serial.print(input);
        }
    }
}

void SerialCommandInterface::executeCommand() {
    if (commandReceived) {
        serialLogger.debug("SerialCommandInterface::executeCommand: '" + inputBuffer + "'");
        Serial.println();
        Serial.println(commandLineInterface.executeCommand(inputBuffer));
        Serial.print("> ");
        commandReceived = false;
        inputBuffer = "";
    }
}

SerialCommandInterface serialCLI;

#pragma clang diagnostic pop