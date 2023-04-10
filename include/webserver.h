#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "sensor_manager.h"
#include "dht22_sensor.h"
#include "hc_sr04_sensor.h"

#define DHTPIN 2

#define SCL_PIN 5
#define SDA_PIN 4

#define ECHO_PIN 13
#define TRIGGER_PIN 12

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