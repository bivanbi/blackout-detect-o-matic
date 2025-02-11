#include "main.h"


void setup() {
    serialLogger.info("main::setup(): initial delay to allow serial monitor to connect");
    delay(3000);
    serialLogger.info("main::setup(): begin");
    persistentStorage.mount();
    initConfiguration();
    initPersistentStorage();
    SystemStatusLoader::load();
    serialCLI.setup();
    initWiFi();
    initNTP();
    systemStatus.rebootDetected(ntpClientAdapter.getUnixTime());
    initPins();
    PeriodicTaskScheduler::init();
    initTelnetServer();
    serialLogger.info("main::setup(): done");
}

void loop() {
    TelnetServer::loop();
    serialCLI.loop();
    PeriodicTaskScheduler::loop();
}

void initConfiguration() {
    serialLogger.info("initConfiguration: load configuration from SD card");
    ConfigurationLoader::load();
    serialLogger.info("initConfiguration: loaded configuration: " + configuration.toJsonDocument().as<String>());
}

void initPersistentStorage() {
    String logDirectory = configuration.getLogDirectory();
    if (!persistentStorage.exists(logDirectory)) {
        persistentStorage.createDirectory(logDirectory);
    }
}

void initWiFi() {
    if (configuration.getWifiSSID().isEmpty()) {
        serialLogger.error("initWiFi: No WiFi SSID configured, skipping WiFi initialization");
        return;
    }

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
    if (configuration.getWifiSSID().isEmpty()) {
        serialLogger.error("initWiFi: No WiFi SSID configured, skipping NTP initialization");
        return;
    }

    serialLogger.info("initNTPClient: Setting NTP server: " + configuration.getNtpServer() + ", update interval: " +
                      configuration.getNtpUpdateInterval() + " seconds, offset: " + configuration.getNtpOffset() +
                      " seconds");
    ntpClientAdapter.setServerName(configuration.getNtpServer());
    ntpClientAdapter.setUpdateInterval(configuration.getNtpUpdateInterval());
    ntpClientAdapter.setTimeOffset(configuration.getNtpOffset());
    PeriodicTaskScheduler::syncTime(true);
}

void initPins() {
    PowerDetectorPin::init();
    ClearAlarmPin::init();
    ResetSystemStatusPin::init();
    AlarmLED::init();
}

void initTelnetServer() {
    if (configuration.getWifiSSID().isEmpty()) {
        serialLogger.error("initWiFi: No WiFi SSID configured, skipping Telnet Server initialization");
        return;
    }
    TelnetServer::begin(configuration.getTelnetServerPort());
}
