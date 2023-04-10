#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "sensor_manager.h"
#include "dht22_sensor.h"
#include "gpio.h"
#include "hc_sr04_sensor.h"

class WebServer {
    public:
    WebServer(std::shared_ptr<StatusReporter> reporter);
    ~WebServer();

    void Start();
    void Update();

    private:
    void InitSensors();
    void SendEvent(JSONVar json, const char* event);

    std::shared_ptr<StatusReporter> reporter;

    SensorManager* sensorManager;
    AsyncWebServer* server;
    AsyncEventSource* events;

    uint64_t prev_event_time;
    const uint64_t event_delay = 1000;  // Send events to subscribers every second
};

#endif