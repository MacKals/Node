//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _ANALOGSENSOR_H
#define _ANALOGSENSOR_H

#include "Sensor.h"

class AnalogSensor : public Sensor {
private:

public:

    AnalogSensor(uint8_t pin) : Sensor(pin) {
        pinMode(pin, INPUT);
    }

    // Sensor methods

    String readDataToString() {
        uint16_t data = this->readData();
        uint8_t lsb = data & 0xff; // keep only lower digits
        uint8_t msb = (data >> 8); // bit shift

        return String(address) + String((char) msb) + String((char) lsb);
    }

    bool sensorPresent() {
        return readData() >= 10;
    }


    // AnalogSensor spesific methods

    uint16_t readData() {
        analogReadAveraging(10);
        return analogRead(pin);
    }
};

#endif
