//  SDISensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _SDISENSOR_H
#define _SDISENSOR_H

#include "Configuration.hpp"

#include "Sensor.h"
#include <SDI12_Teensy35.h>

class SDISensor : public Sensor {
private:
    SDI12 sdiBus;
    const uint8_t pin;

    String sensorAddresses = "";

public:

    SDISensor(char address, uint8_t pin)
     : Sensor(address), sdiBus(pin), pin(pin) {}

    // Sensor methods
    String readDataToString();
    bool sensorPresent();

    // SDISensor spesific methods
public:
    String getAllActiveAddresses();
    void printInfo(char i);
    void init(); // must be called before use

private:
    String printBufferToString();
    String takeMeasurement(char i);
    bool checkActive(char i);
    bool changeAddress(char from, char to);
    bool addressAttached(char i);
};

#endif
