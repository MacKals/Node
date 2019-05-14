//  Sensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _SENSOR_H
#define _SENSOR_H

#include <Arduino.h>

class Sensor {

public:
    const uint8_t pin;     // digital pin number of connected sensor
    const uint8_t address; // address assigned to device on our network

    // Default constructor, address = pin
    Sensor(uint8_t pin) : pin(pin), address(pin) {}

    // Return data read by sensor as a string.
    // Format: data as strings, entries separated by commas
    virtual String readDataToString() = 0;

    // function() = 0 is called a virtual function. Forces subclasses to implement.
};

#endif
