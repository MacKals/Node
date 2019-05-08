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

    String readDataToString() {
        String data = String(getAverageFlow()) + "," + String(getTotalFlow());
        reset();
        return data;
    }

    bool sensorPresent() {
        uint32_t val = analogRead(pin);
        if (val < 8) return true;
        if (val == 1023) return true;
        return false;
    }

    
    // AnalogSensor spesific methods

    bool init();

    float getTotalFlow() {
        return count;
    }

    float getAverageFlow() {
        time_t deltaT = Teensy3Clock.get() - currentCountStartTime;
        return ((float) count) / deltaT;
    }

    void reset() {
        // set up for next measurnment
        count = 0;
        currentCountStartTime = Teensy3Clock.get();
    }
};

#endif
