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

#include <vector>

class EcoSensors {
private:
    const uint8_t analogPins[4] = {A7, A6, A9, A8};
    // const uint8_t sdiPins[1] = {3};
    const uint8_t sdiPins[4] = {2, 3, 17, 22}; //, 33};
    const uint8_t flowPins[2] = {};

    // must include array sizes

    std::vector<Sensor*> sensors;

    void attachAnalogSensors();
    void attachSDISensors();
    void attachFlowSensors();

public:

    void init() {
        //this->attachAnalogSensors();
        this->attachSDISensors();
        // this->attachFlowSensors();

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
