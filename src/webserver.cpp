#include <ESPAsyncWebServer.h>

#include "webserver.h"

WebServer::WebServer() {
    server = new AsyncWebServer(80);
    events = new AsyncEventSource("/events");
}

void WebServer::Start() {

    InitSensors();

    prev_event_time = millis();

    server->serveStatic("/", LittleFS, "/");

    // Web Server Root URL
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Request for the latest sensor readings
    server->on("/readings", HTTP_GET, [&sensorManager = sensorManager](AsyncWebServerRequest *request) {
        String json = JSON.stringify(sensorManager.GetReadings());
        request->send(200, "application/json", json);
        json = String();
    });

    events->onConnect([](AsyncEventSourceClient *client) {
        if (client->lastId()) {
            Serial.printf(
                "Client reconnected! Last message ID that it got is: %u\n",
                client->lastId());
        } else {
            Serial.println("New client connection");
        }
        // send event with message "hello!", id current millis
        // and set reconnect delay to 10 seconds
        client->send("hello!", NULL, millis(), 10000);
    });

    server->addHandler(events);

    // server->end();

    // Start server
    server->begin();
}

void WebServer::Update() {
    sensorManager.UpdateSensors();

    if (millis() - prev_event_time > event_delay) {
        SendEvent(sensorManager.GetReadings(), "new_readings");
        prev_event_time = millis();
    }
}

WebServer::~WebServer() {
    delete events;
    delete server;
}

/* PRIVATE METHODS */

void WebServer::InitSensors() {
    sensorManager.AddSensor(new DHT22Sensor(DHTPIN));
}

void WebServer::SendEvent(JSONVar json, const char* event) {
    events->send(JSON.stringify(json).c_str(), event, millis());
}