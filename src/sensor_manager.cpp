
#include "sensor_manager.h"

SensorManager::SensorManager() {
    num_sensors = 0;
}

SensorManager::~SensorManager() {
    for (int i = 0; i < num_sensors; i++) {
        delete sensors[i];
    }
}

bool SensorManager::AddSensor(ISensor* sensor) {
    if (num_sensors < 16 && sensor->Init()) {
        sensors[num_sensors++] = sensor;
        return true;
    }

    delete sensor;
    return false;
}

void SensorManager::UpdateSensors() {
    for (int i = 0; i < num_sensors; i++) {
        ISensor* sensor = sensors[i];
        if (sensor->IsReady()) {
            sensor->Update();
        }
    }
}

JSONVar SensorManager::GetReadings() const {
    JSONVar readings;
    for (int i = 0; i < num_sensors; i++) {
        ISensor* sensor = sensors[i];
        sensor->GetReadings(&readings);
    }
    return readings;
}
