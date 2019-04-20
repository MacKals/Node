//  EcoNode.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECONODE_H
#define _ECONODE_H

#include "Configuration.hpp"
#include "EcoRadio.hpp"
#include "EcoSensor.hpp"
#include <Arduino.h>
#include <list>

#include <TimeLib.h> // sync RTC


class EcoNode {

    EcoRadio radio;
	EcoSensor sensors = EcoSensor();

	int timeLastData;

public:

EcoNode();

    void init();

	void loop() {
		this->radio.loop();

		// send message every 10 seconds
		if (timerDone()) {
			this->sendData();
			this->startTimer(60);
		}
	}

	void sendData() {
		PRINT("sending data");
		this->radio.send(sensors.readAllSensors());
	}


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
