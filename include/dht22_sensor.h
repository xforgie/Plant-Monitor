#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <memory>
#include <time.h>

#include "isensor.h"
#include "status_reporter.h"

class DHT22Sensor : public ISensor {
   public:
    DHT22Sensor(std::shared_ptr<StatusReporter> reporter, const int pin);
    ~DHT22Sensor();

    virtual bool Init();
    virtual bool Update();
    virtual bool IsReady() const;

    virtual void GetReadings(JSONVar* readings) const;
    virtual std::string GetName();
    inline std::string ToString() const;

   private:
    std::shared_ptr<StatusReporter> reporter;

    int data_pin;
    uint32_t delay;
    uint64_t prev_time;

    DHT_Unified* dht;
    sensor_t temperature_sensor;
    sensor_t humidity_sensor;
    float temperature;
    float humidity;
};

#endif