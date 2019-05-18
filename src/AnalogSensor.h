//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _ANALOGSENSOR_H
#define _ANALOGSENSOR_H

#include "Sensor.h"

class AnalogSensor : public Sensor {

public:

    AnalogSensor(uint8_t pin, String serialNum) : Sensor(pin, serialNum) {
        pinMode(pin, INPUT);
        analogReadRes(13); // 13 bit analog input, 0V -> 0, 3.3V -> 8192
    }

    // Sensor methods

    String readDataToString() {
        return String(readData()/3.3); // volts

        // uint16_t data = this->readData();
        // uint8_t lsb = data & 0xff; // keep only lower digits
        // uint8_t msb = (data >> 8); // bit shift
        // return String(address) + String((char) msb) + String((char) lsb);
    }


    // AnalogSensor specific methods

    float readData() {
        analogReadAveraging(10);
        return analogRead(pin);
    }
};

#endif
