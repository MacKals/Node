//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _FLOWSENSOR_H
#define _FLOWSENSOR_H

#include "Sensor.h"
#include "Configuration.hpp"

class FlowSensor : public Sensor {
private:

    uint32_t currentCountStartTime = 0; // us

    bool initialized = false;

public:
    volatile uint32_t count = 0; // sense count, accessed by ISR

public:
    // Sensor methods
    FlowSensor(uint8_t pin) : Sensor(pin) {}
    ~FlowSensor();

    String readDataToString();

    // AnalogSensor spesific methods
    bool init();
    float getTotalFlow();
    float getAverageFlow();
    void reset();
};

#endif
