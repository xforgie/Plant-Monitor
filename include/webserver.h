#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "sensor_manager.h"
#include "dht22_sensor.h"

#define DHTPIN 5  // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

class WebServer {
    public:
    WebServer();
    ~WebServer();

    void Start();
    void Update();

    private:
    void InitSensors();
    void SendEvent(JSONVar json, const char* event);

    SensorManager sensorManager;
    AsyncWebServer* server;
    AsyncEventSource* events;

    uint64_t prev_event_time;
    const uint64_t event_delay = 1000;  // Send events to subscribers every second
};

#endif