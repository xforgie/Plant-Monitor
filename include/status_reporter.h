#ifndef STATUS_REPORTER_H
#define STATUS_REPORTER_H

#include <Arduino.h>
#include <map>
#include <string>
#include <deque>

#include "gpio.h"

enum Status {STATUS_OK = 0, STATUS_WARN = 1, STATUS_ERROR = 2};

class StatusReporter {
    public:
    StatusReporter() {
        pinMode(RED_LED_PIN, OUTPUT);
        pinMode(YELLOW_LED_PIN, OUTPUT);
        pinMode(GREEN_LED_PIN, OUTPUT);

        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
    };
    ~StatusReporter() {};

    void PostEvent(std::string identifier, Status status, std::string msg) {
        status_table[identifier] = Log {identifier, status, msg, millis()}; 
        Serial.printf("[%08lu] [%s] [%s] %-24s \n", 
            status_table[identifier].time, 
            identifier.c_str(), 
            StringifyStatus(status).c_str(), 
            msg.c_str());
        UpdateLEDs();
    };

    void PrintTableToSerial() {
        Serial.printf("------- STATUS REPORT BEGIN -------\n");
        for (std::pair<std::string, Log> pair: status_table) {
            Log log = pair.second;
            Serial.printf("[%08lu] [%s] [%s] %-24s \n", 
                log.time, 
                log.identifier.c_str(), 
                StringifyStatus(log.status).c_str(), 
                log.msg.c_str());
        }
        Serial.printf("-------- STATUS REPORT END --------\n");
    }

    private:
    typedef struct Log {
        std::string identifier;
        Status status;
        std::string msg;
        unsigned long time;
    } Log;
    
    std::map<std::string, Log> status_table;

    const std::string StringifyStatus(Status status) const {
        switch (status) {
            case STATUS_OK: return "OK";
            case STATUS_WARN: return "WARN";
            case STATUS_ERROR: return "ERROR";
            default: return "NONE";
        }
    };

    const void UpdateLEDs() {
        Status max_status = Status::STATUS_OK; // 0 = OK, 1 = WARN, 2 = ERROR
        for (std::pair<std::string, Log> row : status_table) {
            Status status = row.second.status;
            max_status = std::max(max_status, status);
        }

        digitalWrite(RED_LED_PIN, max_status == Status::STATUS_ERROR ? LOW : HIGH);
        digitalWrite(YELLOW_LED_PIN, max_status == Status::STATUS_WARN ? HIGH : LOW);
        digitalWrite(GREEN_LED_PIN, max_status == Status::STATUS_OK ? HIGH : LOW);
    }
};

#endif