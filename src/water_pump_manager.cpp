
#include "water_pump_manager.h"

WaterPumpManager::WaterPumpManager(const int gpio_pin) {
    this->gpio_pin = gpio_pin;
    state = LOW;

    pinMode(gpio_pin, OUTPUT);
    digitalWrite(gpio_pin, state);
}

void WaterPumpManager::Update() {
    if (state == HIGH)
        analogWrite(gpio_pin, flow_rate);
    else
        analogWrite(gpio_pin, 0);
}

void WaterPumpManager::SetFlowRate(const uint8_t flow_rate) {
    this->flow_rate = flow_rate;
}

void WaterPumpManager::SetState(const bool state) {
    this->state = state;
}

WaterPumpManager::~WaterPumpManager() {

}