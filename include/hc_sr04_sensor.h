#ifndef HC_SR04_SENSOR_H
#define HC_SR04_SENSOR_H

#include <Arduino.h>
#include <Arduino_JSON.h>

#include "isensor.h"

class HC_SR04Sensor : public ISensor {
    public:
    HC_SR04Sensor(const int trig, const int echo);
    ~HC_SR04Sensor();

    virtual bool Init();
    virtual bool Update();
    virtual bool IsReady() const;
    virtual String GetName();

    virtual void GetReadings(JSONVar* readings) const;

    private:

    enum State {INIT, WRITING, READING};

    int trigger_pin;
    int echo_pin;

    uint32_t delay;
    unsigned long prev_time;
    State state;
    float distance;
};

#endif