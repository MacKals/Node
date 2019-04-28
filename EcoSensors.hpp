//  EcoSensors.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#ifndef _ECOSENSORS_H
#define _ECOSENSORS_H

#include "Configuration.hpp"

#include "Sensor.h"
#include "AnalogSensor.h"
#include "SDISensor.h"
#include "FlowSensor.h"

#include <vector>

class EcoSensors {
private:
    const uint8_t analogPins[4] = {A7, A6, A9, A8};
    const uint8_t sdiPins[2] = {33, 3};
    // must include array sizes

    std::vector<Sensor> sensors;

    void attachAnalogSensors();
    void attachSDISensors();

public:

    void init() {
        this->attachAnalogSensors();
        this->attachSDISensors();
    }

    String getFullDataString() {
        String data = "";
        // TODO: include time-stamp

        // loop through sensors using itterators
        for (auto sensor = this->sensors.begin();
             sensor != this->sensors.end();
             ++sensor) {
            data += sensor->readDataToString();
        }
        return data;
    }
};


#endif
