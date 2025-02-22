#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include "SerialCommandInterface.h"

const String SerialCommandInterface::logTag = "SerialCommandInterface: ";

void SerialCommandInterface::setup() {
    Serial.begin(115200);
    Logger::info(logTag + "ready to receive commands", {Logger::Channel::SERIAL_PORT});
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
        } else if (input == '\n') {
            commandReceived = true;
            Logger::debug(logTag + "readSerial: Received command: '" + inputBuffer + "'", {Logger::Channel::SERIAL_PORT});
        } else {
            inputBuffer += input;
            Serial.print(input);
        }
    }
}

void SerialCommandInterface::executeCommand() {
    if (commandReceived) {
        Logger::debug(logTag + "executeCommand: '" + inputBuffer + "'");
        Serial.println();
        Serial.println(commandLineInterface.executeCommand(inputBuffer));
        Serial.print("> ");
        commandReceived = false;
        inputBuffer = "";
    }
}

SerialCommandInterface serialCLI;

#pragma clang diagnostic pop