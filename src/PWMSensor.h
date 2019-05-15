//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _PWMSENSOR_H
#define _PWMSENSOR_H

#include "Sensor.h"

class PWMSensor : public Sensor {
private:

public:

    PWMSensor(uint8_t pin, uint32_t serialNum) : Sensor(pin, serialNum) {
        pinMode(pin, INPUT);
    }

    // Sensor methods

    String readDataToString() {
        uint16_t data = readDataAs16Bit();
        uint8_t lsb = data & 0xff; // keep only lower digits
        uint8_t msb = (data >> 8); // bit shift

        return String(pin) + String((char) msb) + String((char) lsb);
    }


    // AnalogSensor specific methods

    uint16_t readDataAs16Bit() {
        return (uint16_t) (readData() * 0xfffe); // 2^16-1
    }

    double readData() {
        float sum = 0;
        const uint8_t count = 10;

        for (int i = 0; i<count; i++) {
            uint32_t tHigh = pulseIn(this->pin, HIGH, 10000); // 10 ms timeout
            uint32_t tLow = pulseIn(this->pin, LOW, 10000);

            if (tHigh && tLow) {
                sum += (float) tHigh / (tLow + tHigh);
            } else {
                return 0;
            }
        }
        return sum/count;
    }
};

#endif
