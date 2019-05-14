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

    AnalogSensor(uint8_t pin) : Sensor(pin) {
        pinMode(pin, INPUT);
        analogReadRes(13); // 13 bit analog input, 0V -> 0, 3.3V -> 8192
    }

    // Sensor methods

    String readDataToString() {
        return String(address) + "," + String(readData());
        // uint16_t data = this->readData();
        // uint8_t lsb = data & 0xff; // keep only lower digits
        // uint8_t msb = (data >> 8); // bit shift
        //
        // return String(address) + String((char) msb) + String((char) lsb);
    }

    bool sensorPresent() {
        uint16_t sampleData1 = analogRead(pin);
        delay(10);
        pinMode(pin, INPUT_PULLUP);
        delay(50);
        uint16_t sampleData2 = analogRead(pin);

        // PRINTLN(String(sampleData1) + " followed by " + String(sampleData2));

        // assume connected if low with pullup
        bool connected = (sampleData2 < 7800);

        // chech if input was high before pullup
        if (abs(sampleData1 - sampleData2) < 50) {
            // pullup made little difference, assume sensor connected
            connected = true;
        }

        pinMode(pin, INPUT);
        return connected;
    }


    // AnalogSensor spesific methods

    float readData() {
        analogReadAveraging(10);
        return analogRead(pin);
    }
};

#endif
