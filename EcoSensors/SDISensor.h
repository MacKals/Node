//  SDISensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _SDISENSOR_H
#define _SDISENSOR_H

#include "Sensor.h"
#include <SDI12_Teensy35.h>

class SDISensor : public Sensor {
private:
    const SDI12 sdiBus;
    const pin pin;

    String sensorAddresses = "";

public:

    SDISensor(char address, uint8_t pin)
     : Sensor(address), sdiBus(pin), pin(pin) {

        // SDI-12 bus must be initialized with init method
        this->sdiBus.init();
        delay(30); // TODO: incrase? decrease?

        this->sensorAddress = this->getAllActiveAddresses(); // TODO: change?
    }

    // Sensor methods
    String readDataToString();
    bool sensorPresent();

    // SDISensor spesific methods
public:
    String getAllActiveAddresses();
    void printInfo(char i);

private:
    String printBufferToString();
    String takeMeasurement(char i);
    bool checkActive(char i);
    bool changeAddress(char from, char to);
    bool addressAttached(char i);
};

#endif
