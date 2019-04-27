//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _ANALOGSENSOR_H
#define _ANALOGSENSOR_H

class AnalogSensor : public Sensor {
private:
    const uint8_t pin;

public:

    AnalogSensor(char address, uint8_t pin)
     : Sensor(address) {
        this->pin = pin;
    }

    uint16_t readData() {
        analogReadAveraging(10);
        return analogRead(pin);
    }

    String readDataToString() {
        uint16_t data = readData();
        uint8_t lsb = data & 0xff; // keep only lower digits
        uint8_t msb = (data >> 8); // bit shift

        char c[] = {this->getAddress(), (char) msb, (char) lsb};
        return String(c);
    }

    bool sensorPresent() {
        return this->readData() >= 10;
    }
};

#endif
