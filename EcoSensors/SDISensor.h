//  SDISensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#ifndef _SDISENSOR_H
#define _SDISENSOR_H

class SDISensor : public Sensor {

public:

    // // return array of all uninitialized sdi sensors
    // static sdiSensor * getAllActiveSensors() {
    //     return ;
    // }

    sdiSensor(char address)
     : Sensor(address) {}

    uint16_t readData() {
        return 0;
    }

    String readDataToString() {
        return "";
    }

    bool sensorPresent() {
        return false;
    }
};

#endif
