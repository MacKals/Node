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
    const vector<uint8_t> threePortPins {15, 16, 17, 18, 19, 20, 21, 35, 36, 37, 38, 39};

    const vector<uint8_t> poweredPins      {18, 19, 20, 21};
    const vector<uint8_t> powerControlPins { 5,  5,  4,  4};

    std::vector<Sensor*> sensors;

    bool pinInUse(uint8_t pin);

    void attachAnalogSensors();
    void attachPWMSensors();
    void attachSDISensors();
    void attachFlowSensors();

    void powerOn(uint8_t pin);
    void powerOff(uint8_t pin);


public:
    void init(EcoSD sd);
    String getFullDataString();
    String getFullConfiguration();
};


#endif
