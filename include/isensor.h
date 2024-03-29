#ifndef ISENSOR_H
#define ISENSOR_H

#include <Arduino_JSON.h>
#include <string>

class ISensor {
    public:
    virtual ~ISensor() {};
    virtual bool Init() = 0;
    virtual bool Update() = 0;
    virtual bool IsReady() const = 0;

    virtual void GetReadings(JSONVar* readings) const = 0;
    virtual std::string GetName() = 0;
};

#endif