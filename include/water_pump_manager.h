#ifndef WATER_PUMP_MANAGER_H
#define WATER_PUMP_MANAGER_H

#include <Arduino.h>

class WaterPumpManager {
    public:
    WaterPumpManager(const int gpio);
    ~WaterPumpManager();

    void Update();
    void SetFlowRate(const uint8_t flow_rate);
    void SetState(const bool state);

    private:
    int gpio_pin;
    bool state;

    uint8_t flow_rate = 255;
};

#endif