//  EcoNode.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECONODE_H
#define _ECONODE_H

#include "Configuration.hpp"
//#include "EcoRadio.hpp"
#include "EcoSensor.hpp"
#include <list>

class EcoNode {

    // EcoRadio radio;
	EcoSensor sensors = EcoSensor();

public:

EcoNode();
    void init();
    std::list<EcoNode> getChildren();
    void collectSensorData();
    std::list<EcoSensor> getSensors();

	void testFunc() {
		sensors.attachAllSDI12();
	}

    private:
    void pollSensor(EcoSensor s);
    int getAddress();
};

#endif
