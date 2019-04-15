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
#include <list>

class EcoNode {

    EcoRadio radio;
    uint8_t nodeAddress;
    uint8_t parentAddress;         // 0 for no current parent

	EcoSensor sensors = EcoSensor();

    // std::list<EcoSensor> sensors;
    // std::list<EcoNode> children;

public:

EcoNode();
    void init();
    std::list<EcoNode> getChildren();
    void collectSensorData();
    std::list<EcoSensor> getSensors();

	void testFunc() {
		sensors.attachAllSensors();
	}

    private:
    void pollSensor(EcoSensor s);
    int getAddress();
};

#endif
