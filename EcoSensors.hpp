//  EcoSensors.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#ifndef _ECOSENSORS_H
#define _ECOSENSORS_H

#include <Arduino.h>
#include "Configuration.hpp"

#include "Sensor.h"
#include "AnalogSensor.h"
#include "SDISensor.h"
#include "FlowSensor.h"
#include "PWMSensor.h"

#include <vector>

class EcoSensors {
private:
    const uint8_t analogPins[5] = {2, 3, 17, 22, 33};
    // const uint8_t sdiPins[1] = {3};
    const uint8_t sdiPins[5] = {2, 3, 17, 22, 33};
    const uint8_t flowPins[0] = {};

    // must include array sizes

    std::vector<Sensor*> sensors;

    bool attachSensorIfPresent(Sensor * s);
    bool pinInUse(uint8_t pin);

    void attachAnalogSensors();
    void attachPWMSensors();
    void attachSDISensors();
    void attachFlowSensors();

public:

    void init() {
        attachPWMSensors();
        attachAnalogSensors();
        attachFlowSensors();
        // attachSDISensors();

        PRINT("Connected sensors: ");
        for (auto s = this->sensors.begin(); s != this->sensors.end(); ++s) {
            PRINT((*s)->address);
            PRINT(" ");
        }
        PRINTLN("");
    }

    String getFullDataString() {

        time_t t = Teensy3Clock.get(); // s, time since 1.1.1970 (unix time)
        // TODO: encode t as bits, not ASCII
        String data = String(t);

        // loop through sensors using itterators
        for (auto s = sensors.begin(); s != sensors.end(); ++s) {
            data += "&" + (*s)->readDataToString();
        }
        return data;
    }
};


#endif
