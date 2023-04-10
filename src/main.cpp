#include <Arduino.h>
#include <WiFiManager.h>
#include <FS.h>
#include <LittleFS.h>

#include "status_reporter.h"
#include "webserver.h"
#include "water_pump_manager.h"

#define FORMAT_LITTLEFS_IF_FAILED true

#define WIFI_TIMEOUT 15

std::shared_ptr<StatusReporter> reporter(new StatusReporter());

WiFiManager wifiManager;
WebServer server(reporter);
// WaterPumpManager waterPump(WATER_PUMP_GPIO);

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname, "r");
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.print(file.name());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",
                          (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1,
                          tmstruct->tm_mday, tmstruct->tm_hour,
                          tmstruct->tm_min, tmstruct->tm_sec);
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.print(file.size());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",
                          (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1,
                          tmstruct->tm_mday, tmstruct->tm_hour,
                          tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

// Initialize WiFi
void initWiFi() {
    WiFi.mode(WIFI_STA);
    wifiManager.setTimeout(WIFI_TIMEOUT);
    reporter->PostEvent("WIFI", Status::STATUS_WARN, "Attempting to connect");
    wifiManager.autoConnect();
    
    if (WiFi.localIP().isSet()) {
        reporter->PostEvent("WIFI", Status::STATUS_OK, "Connected");
        Serial.println("Connection established!");
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        reporter->PostEvent("WIFI", Status::STATUS_ERROR, "WiFi not connected");
    }
}

// Initialize LittleFS
void initFS() {
    if (!LittleFS.begin()) {
        reporter->PostEvent("LFS", Status::STATUS_ERROR, "An error has occurred while mounting LittleFS");
        return;
    }
    reporter->PostEvent("LFS", Status::STATUS_OK, "LittleFS mounted successfully");
    Serial.println("---- list ----");
    listDir(LittleFS, "/", 1);

    File file = LittleFS.open("/index.html", "r");
    if (!file) {
        reporter->PostEvent("LFS", Status::STATUS_ERROR, "Failed to open file for reading");
    }
    file.close();
}

void setup() {
	Serial.begin(115200);
    
	initWiFi();
    initFS();

    server.Start();

    reporter->PostEvent("SERVER", Status::STATUS_OK, "Web server connected");

    reporter->PrintTableToSerial();
}

void loop() {
    server.Update();
    // waterPump.Update();
}