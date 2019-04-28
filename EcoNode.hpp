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

#include <Arduino.h>
#include <list>

#include <TimeLib.h> // sync RTC


class EcoNode {

    EcoRadio radio;
	EcoSensors sensors;

	int timeLastData;

public:

    void init();
    void loop();

    void sendData();

//     void collectSensorData();
//     bool transmitDataFromMemory(uint8_t pa);
//
// private:
// 	void pollSensor(EcoSensor s);
	int getAddress();

	// TIME
	time_t currentTimeInSeconds();
	void setRTC();
	String timeAsString();

	// delay time in seconds, false if timer allready set
	bool startTimer(uint8_t time);
	bool timerDone();
private:
	time_t timerTime;
};

#endif
