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
#include <vector>

using namespace std;

class SDISensor : public Sensor {
private:
    SDI12 sdiBus;
    String sensorAddresses = "";

public:
    SDISensor(uint8_t pin, String serialNum) : Sensor(pin, serialNum), sdiBus(pin) {}

    // Sensor methods
    String readDataToString();

    // SDISensor spesific methods
    char getFirstActiveAddress();
    String getAllActiveAddresses();
    String printInfoToString(char i);
    void printInfo();

    void init(); // must be called before use
    void end();

private:
    String printBufferToString();
    String takeMeasurement(char i);
    bool checkActive(char i);
    void changeAddress(char from, char to);
    bool addressAttached(char i);
};

#endif
