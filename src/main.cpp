#include <Arduino.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <time.h>

#include "webserver.h"

#define FORMAT_LITTLEFS_IF_FAILED true

WebServer webServer;

// SSID and Password of your WiFi router
const char *ssid = "********";
const char *password = "********";
AsyncWebServer server(80);           // Server on port 80
AsyncEventSource events("/events");  // Create an Event Source on /events

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

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
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
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