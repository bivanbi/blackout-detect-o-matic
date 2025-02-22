#include "OTAAdapter.h"

const String OTAAdapter::logTag = "OTAAdapter: ";

void OTAAdapter::init() {
    Logger::info(logTag + "init begin");
    String password = configuration.getOTAPassword();

    ArduinoOTA.setPassword(configuration.getOTAPassword().c_str());
    ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH) {
                    type = "sketch";
                } else {  // U_SPIFFS
                    type = "filesystem";
                }

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                Logger::info(logTag + "Start updating " + type);
            })
            .onEnd([]() {
                Logger::info(logTag + "end");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Logger::debug(logTag + "Progress: " + String (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Logger::error(logTag + "there was an error");
                if (error == OTA_AUTH_ERROR) {
                    Logger::error(logTag + "Auth Failed");
                } else if (error == OTA_BEGIN_ERROR) {
                    Logger::error(logTag + "Begin Failed");
                } else if (error == OTA_CONNECT_ERROR) {
                    Logger::error(logTag + "Connect Failed");
                } else if (error == OTA_RECEIVE_ERROR) {
                    Logger::error(logTag + "Receive Failed");
                } else if (error == OTA_END_ERROR) {
                    Logger::error(logTag + "End Failed");
                }
            });

    ArduinoOTA.begin();
    Logger::info(logTag + "init end");
};

void OTAAdapter::loop() {
    ArduinoOTA.handle();
};
