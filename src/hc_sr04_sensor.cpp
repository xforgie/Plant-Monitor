
#include "hc_sr04_sensor.h"

HC_SR04Sensor::HC_SR04Sensor(const int trig, const int echo) {
    
    trigger_pin = trig;
    echo_pin = echo;
    
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    state = State::INIT;
    distance = 0;
}

HC_SR04Sensor::~HC_SR04Sensor() {

}

bool HC_SR04Sensor::Init() {
    prev_time = micros();
    delay = 0;

    return true;
}

bool HC_SR04Sensor::Update() {
    switch (state) {
        case INIT: 
            digitalWrite(trigger_pin, LOW);
            delay = 5;
            state = WRITING;
            break;
        case WRITING:
            digitalWrite(trigger_pin, HIGH);
            delay = 10;
            state = READING;
            break;
        case READING:
            digitalWrite(trigger_pin, LOW);
            unsigned long duration = pulseIn(echo_pin, HIGH);
            distance = (duration/2) / 29.1f;

            // Serial.printf("Distance: %.2f cm\n", distance);

            delay = 1000000ul;  // wait 1 second for another reading
            state = INIT;
            break;
    }    

    prev_time = micros();

    return true;
}

bool HC_SR04Sensor::IsReady() const {
    return micros() - prev_time >= delay;
}

String HC_SR04Sensor::GetName() {
    return String("HC_SR04");
}

void HC_SR04Sensor::GetReadings(JSONVar* readings) const {
    (*readings)["water_level"] = String(distance);
}