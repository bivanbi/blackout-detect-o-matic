#include "OTAAdapter.h"

void OTAAdapter::init() {
    Logger::info("OTA: init begin");
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
                Logger::info("OTA: Start updating " + type);
            })
            .onEnd([]() {
                Logger::info("OTA: end");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Logger::debug("OTA: Progress: " + String (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Logger::error("OTA: there was an error");
                if (error == OTA_AUTH_ERROR) {
                    Logger::error("OTA: Auth Failed");
                } else if (error == OTA_BEGIN_ERROR) {
                    Logger::error("OTA: Begin Failed");
                } else if (error == OTA_CONNECT_ERROR) {
                    Logger::error("OTA: Connect Failed");
                } else if (error == OTA_RECEIVE_ERROR) {
                    Logger::error("OTA: Receive Failed");
                } else if (error == OTA_END_ERROR) {
                    Logger::error("OTA: End Failed");
                }
            });

    ArduinoOTA.begin();
    Logger::info("OTA: init end");
};

void OTAAdapter::loop() {
    ArduinoOTA.handle();
};
