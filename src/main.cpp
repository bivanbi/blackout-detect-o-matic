#include "main.h"

void setup() {
    Logger::begin();
    Logger::info("main::setup(): initial delay to allow serial monitor to connect", {Logger::Channel::SERIAL_PORT});
    delay(3000);
    Logger::info("main::setup(): begin", {Logger::Channel::SERIAL_PORT});
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
    Logger::info("main::setup(): done");
}

void loop() {
    TelnetServer::loop();
    serialCLI.loop();
    PeriodicTaskScheduler::loop();
}

void initConfiguration() {
    Logger::info("initConfiguration: load configuration from SD card", {Logger::Channel::SERIAL_PORT});
    ConfigurationLoader::load();
    Logger::info("initConfiguration: loaded configuration: " + configuration.toJsonDocument().as<String>(),
                 {Logger::Channel::SERIAL_PORT});
}

void initPersistentStorage() {
    String logDirectory = configuration.getLogDirectory();
    if (!persistentStorage.exists(logDirectory)) {
        persistentStorage.createDirectory(logDirectory);
    }
}

void initWiFi() {
    if (configuration.getWifiSSID().isEmpty()) {
        Logger::error("initWiFi: No WiFi SSID configured, skipping WiFi initialization");
        return;
    }

    wifiClientAdapter.connect(configuration.getWifiSSID(), configuration.getWifiSecret());
    while (!wifiClientAdapter.isConnected()) {
        Logger::info("Init: Connecting to WiFi network " + String(WIFI_SSID) + ", mac address: " +
                          String(wifiClientAdapter.getMacAddress()));
        delay(1000);
    }
    Logger::info("Init: Connected to WiFi network " + String(WIFI_SSID) + ", IP address: " +
                      String(wifiClientAdapter.getIpAddress()));
}

void initNTP() {
    if (configuration.getWifiSSID().isEmpty()) {
        Logger::error("initWiFi: No WiFi SSID configured, skipping NTP initialization");
        return;
    }

    Logger::info("initNTPClient: Setting NTP server: " + configuration.getNtpServer() + ", update interval: " +
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
        Logger::error("initWiFi: No WiFi SSID configured, skipping Telnet Server initialization");
        return;
    }
    TelnetServer::begin(configuration.getTelnetServerPort());
}
