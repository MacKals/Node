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
    const uint8_t threePortPins[4] = {2, 3, 17, 22};
    // const uint8_t threePortPins[3] = {33, 34, 35};

    // must include array sizes

    std::vector<Sensor*> sensors;

    bool attachSensorIfPresent(Sensor * s);
    bool pinInUse(uint8_t pin);

    void attachAnalogSensors();
    void attachPWMSensors();
    void attachSDISensors();
    void attachFlowSensors();

public:
    void init();
    String getFullDataString();
};


#endif