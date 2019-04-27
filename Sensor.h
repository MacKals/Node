//  Sensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _SENSOR_H
#define _SENSOR_H

#include <Arduino.h>

class Sensor {

protected:
    const char address; // sensor address

public:
    // Return address of sensor as a character
    char getAddress() {
        return address;
    }

    // Default constructor
    Sensor(char address) : address(address) {}

    // Return data read by sensor as a string. Start with address followed b
    // by comma and information
    virtual String readDataToString()=0;

    // Return true if a sensor is found to be connected
    virtual bool sensorPresent()=0;

    // function() = 0 is called a virtual function. Forces subclasses to implement.
};

#endif
