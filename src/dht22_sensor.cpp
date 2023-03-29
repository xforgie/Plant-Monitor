#include "dht22_sensor.h"

DHT22Sensor::DHT22Sensor(const int pin) {
    data_pin = pin;
}

DHT22Sensor::~DHT22Sensor() {
    delete this->dht;
}

bool DHT22Sensor::Init() {
    
    this->dht = new DHT_Unified(data_pin, 22U);

    dht->begin();
    dht->temperature().getSensor(&this->temperature_sensor);
    dht->humidity().getSensor(&this->humidity_sensor);

    this->delay = std::max(temperature_sensor.min_delay, humidity_sensor.min_delay) / 1000;

    return true;
}

bool DHT22Sensor::Update() {
    // Get temperature event and print its value.
    sensors_event_t event;
    dht->temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        // TODO: handle err
    } else {
        temperature = event.temperature;
    }

    // Get humidity event and print its value.
    dht->humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        // TODO: handle err
    } else {
        humidity = event.relative_humidity;
    }

    return true;
}

inline bool DHT22Sensor::IsReady() const {
    return millis() - prev_time >= delay;
}

void DHT22Sensor::GetReadings(JSONVar* readings) const {
    (*readings)["temperature"] = String(temperature);
    (*readings)["humidity"] = String(humidity);
}

String DHT22Sensor::GetName() {
    return String("DHT22");
}

inline String DHT22Sensor::ToString() const {
    char buffer[1024];
    snprintf(buffer, 1024, R"-----(
        ------------------------------
        Temperature Sensor
        Sensor Type: %s
        Driver Version: %d
        Unique ID: %d
        Max Value: %f°C
        Min Value: %f°C
        Resolution: %f°C
        ------------------------------
        Humidity Sensor
        Sensor Type: %s
        Driver Version: %d
        Unique ID: %d
        Max Value: %f°C
        Min Value: %f°C
        Resolution: %f°C
        ------------------------------
                    )-----", 
                    temperature_sensor.name,
                    temperature_sensor.version,
                    temperature_sensor.sensor_id,
                    temperature_sensor.max_value,
                    temperature_sensor.min_value,
                    temperature_sensor.resolution,
                    humidity_sensor.name,
                    humidity_sensor.version,
                    humidity_sensor.sensor_id,
                    humidity_sensor.max_value,
                    humidity_sensor.min_value,
                    humidity_sensor.resolution);
    
    return String(buffer);
}