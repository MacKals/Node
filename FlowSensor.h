//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _FLOWSENSOR_H
#define _FLOWSENSOR_H

#include "Sensor.h"

// interrupt must be attached by class, something like this:
// attachInterrupt(this->pin, this->meterISR, RISING);


class FlowSensor : public Sensor {
private:

    volatile uint32_t flowCount = 0; // accessed by ISR
    uint32_t currentCountStartTime = 0; // us

    void meterISR() {
        flowCount++; // let meter count the pulses
    }

public:

    FlowSensor(uint8_t pin) : Sensor(pin) {}
    const uint32_t period = 1000; // ms, 1s between measurement update
    // Sensor methods

    String readDataToString() {
        return String(this->readData());
    }

    bool sensorPresent() {
        // todo: how to determine this? Wait and see if ticks happen?
        return true;
    }


    // AnalogSensor spesific methods

    float readData() {

        time_t deltaT = millis() - currentCountStartTime;
        float ticksPerSecond = ((float) this->flowCount) / deltaT;

        // set up for next measurnment
        this->flowCount = 0;
        this->currentCountStartTime = millis();

        return ticksPerSecond;
    }
};

#endif
