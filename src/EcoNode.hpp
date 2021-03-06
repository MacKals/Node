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
#include <Snooze.h>

#include <list>


class EcoNode {

    EcoTimer dataTimer;
    EcoTimer radioTimer;
    EcoTimer gpsTimer;

    EcoRadio radio;
	EcoSensors sensorMaster;
    EcoSD sd;
    EcoGPS gps;

    SnoozeAlarm	alarm;
    SnoozeBlock *config_teensy35;

    uint16_t bootCount;

	time_t timeLastData;

    void initBootCount();
    void setLoRaParameters();
    void setSensorParameters();

    void recordDataPacket();
    void recordConfiguraton();
    void sendDataPacket();

	int getAddress();
    void setRTC();

    // LED
    bool ledStatus = false;
    void blinkLED();
    void activateLED(bool state = true);

public:
    void init();
    void loop();
};

#endif
