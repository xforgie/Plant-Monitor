#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <time.h>

#include "isensor.h"

class DHT22Sensor : public ISensor {
   public:
    DHT22Sensor(const int pin);
    ~DHT22Sensor();

    virtual bool Init();
    virtual bool Update();
    virtual bool IsReady() const;
    virtual String GetName();

    virtual void GetReadings(JSONVar* readings) const;
    inline String ToString() const;

   private:
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