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

#include <Arduino.h>
#include <list>



class EcoNode {

    EcoRadio radio;
	EcoSensors sensors;
    EcoTimer timer;
    EcoSD sd;

	int timeLastData;

    void setLoRaParameters();
    String cleanupString(String s);

public:

    void init();
    void loop();

    void sendData(String data);

//     void collectSensorData();
//     bool transmitDataFromMemory(uint8_t pa);
//
// private:
// 	void pollSensor(EcoSensor s);
	int getAddress();

    void setRTC();
};

#endif