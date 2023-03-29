#include <Arduino.h>
#include <WiFiManager.h>
#include <FS.h>
#include <LittleFS.h>

#include "webserver.h"

#define FORMAT_LITTLEFS_IF_FAILED true

WiFiManager wifiManager;
WebServer webServer;

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
    if (wifiManager.autoConnect()) {
        Serial.println("Connection established!");
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Configuration portal running...");
    }
}

// Initialize LittleFS
void initFS() {
    if (!LittleFS.begin()) {
        Serial.println("An error has occurred while mounting LittleFS");
    }
    Serial.println("LittleFS mounted successfully");
    Serial.println("---- list ----");
    listDir(LittleFS, "/", 1);

    File file = LittleFS.open("/index.html", "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
    }
    file.close();
}

void setup() {
	Serial.begin(115200);
    
	initWiFi();
    initFS();

    webServer.Start();
}

void loop() {
    webServer.Update();
}