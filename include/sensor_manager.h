#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino_JSON.h>
#include <memory>

#include "isensor.h"
#include "status_reporter.h"

class SensorManager {
    public:
    SensorManager(std::shared_ptr<StatusReporter> reporter);
    ~SensorManager();

    bool AddSensor(ISensor* sensor);
    void UpdateSensors();
    JSONVar GetReadings() const;
    // bool RemoveSensor()

    private:
    std::shared_ptr<StatusReporter> reporter;

    int num_sensors;
    ISensor* sensors[16];
};

#endif