#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino_JSON.h>

#include "isensor.h"

class SensorManager {
    public:
    SensorManager();
    ~SensorManager();

    bool AddSensor(ISensor* sensor);
    void UpdateSensors();
    JSONVar GetReadings() const;
    // bool RemoveSensor()

    private:
    int num_sensors;
    ISensor* sensors[16];
};

#endif