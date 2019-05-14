//  EcoNode.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECONODE_H
#define _ECONODE_H

#include "Configuration.hpp"

#include "EcoRadio.hpp"
#include "EcoSensors.hpp"
#include "EcoTimer.hpp"
#include "EcoSD.hpp"
#include "EcoGPS.hpp"

#include <Arduino.h>
#include <EEPROM.h>
#include <list>


class EcoNode {

    EcoTimer dataTimer;
    EcoTimer radioTimer;

    EcoRadio radio;
	EcoSensors sensorMaster;
    EcoSD sd;
    EcoGPS gps;

    uint16_t bootCount;

	int timeLastData;

    void initBootCount();
    void setLoRaParameters();
    void setSensorParameters();
    String cleanupString(String s);

    void recordDataPacket();
    void sendDataPacket();

	int getAddress();
    void setRTC();

    // LED
    bool ledStatus = false;
    void blinkLED();
    void activateLED(bool on = true);

public:
    void init();
    void loop();
};

#endif
