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
#include "EcoSD.hpp"

#include <vector>
#include <tuple>


class EcoSensors {
private:
    const vector<uint8_t> threePortPins {14, 15, 16, 17, 18, 19, 20, 35, 36, 37, 38, 39};

    // must include array sizes

    std::vector<Sensor*> sensors;

    bool attachSensorIfPresent(Sensor * s);
    bool pinInUse(uint8_t pin);

    void attachAnalogSensors();
    void attachPWMSensors();
    void attachSDISensors();
    void attachFlowSensors();

public:
    void init(EcoSD sd);
    String getFullDataString();
    bool initSensorFromString(String s);
};


#endif
