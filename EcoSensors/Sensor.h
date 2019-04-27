//  Sensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _SENSOR_H
#define _SENSOR_H

class Sensor {

protected:
    const char address; // sensor address

public:
    // Return address of sensor as a character
    char getAddress() {
        return address;
    }

    // Default constructor
    Sensor(char address) {
        this->address = address;
    }

    // Return data read by sensor as a string. Start with address followed b
    // by comma and information
    virtual String readDataToString();

    // Return true if a sensor is found to be connected
    virtual bool sensorPresent();
};

#endif
