#include "main.h"


void setup() {
    serialLogger.info("main::setup(): initial delay to allow serial monitor to connect");
    delay(3000);
    serialLogger.info("main::setup(): begin");
    initConfiguration();
    initWiFi();
    initNTP();
    initSystemStatus();
    systemStatus.rebootDetected(ntpClientAdapter.getUnixTime());
    initPins();
    PeriodicTaskScheduler::init();
    initTelnetServer();
    serialLogger.info("main::setup(): done");
}

void loop() {
    TelnetServer::loop();
    PeriodicTaskScheduler::loop();
}

void initConfiguration() {
    serialLogger.info("initConfiguration: TODO: load configuration from SD card into temporary object");
    if (OVERWRITE_CONFIGURATION) {
        serialLogger.info("initConfiguration: TODO: Update configuration on SD card");
        // TODO: compare configuration with sdcard content (if available)
        // TODO: overwrite configuration if needed
    } else {
        // TODO: apply configuration from sdcard if available
        serialLogger.info("initConfiguration: TODO: apply configuration from SD card");
    }

    serialLogger.info("Configuration: " + configuration.toJsonDocument().as<String>());
}

void initWiFi() {
    wifiClientAdapter.connect(configuration.getWifiSSID(), configuration.getWifiSecret());
    while (!wifiClientAdapter.isConnected()) {
        serialLogger.info("Init: Connecting to WiFi network " + String(WIFI_SSID) + ", mac address: " +
                          String(wifiClientAdapter.getMacAddress()));
        delay(1000);
    }
    serialLogger.info("Init: Connected to WiFi network " + String(WIFI_SSID) + ", IP address: " +
                      String(wifiClientAdapter.getIpAddress()));
}

void initNTP() {
    serialLogger.info("initNTPClient: Setting NTP server: " + configuration.getNtpServer() + ", update interval: " +
                      configuration.getNtpUpdateInterval() + " seconds, offset: " + configuration.getNtpOffset() +
                      " seconds");
    ntpClientAdapter.setServerName(configuration.getNtpServer());
    ntpClientAdapter.setUpdateInterval(configuration.getNtpUpdateInterval());
    ntpClientAdapter.setTimeOffset(configuration.getNtpOffset());
    PeriodicTaskScheduler::syncTime(true);
}

void initSystemStatus() {
    serialLogger.error("initSystemStatus: TODO: load system status from SD card");
}

void initPins() {
    PowerDetectorPin::init();
    ClearAlarmPin::init();
    ResetSystemStatusPin::init();
    AlarmLED::init();
}

void initTelnetServer() {
    TelnetServer::begin(configuration.getTelnetServerPort());
}

String getRamInfoString() {
    // TODO make it a JSON object
    return "Total heap: " + String(ESP.getHeapSize())
           + ", tree heap: " + String(ESP.getFreeHeap())
           + ", total PSRAM: " + String(ESP.getPsramSize())
           + ", free PSRAM: " + String(ESP.getFreePsram());
}
