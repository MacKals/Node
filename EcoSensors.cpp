//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"

void EcoSensors::attachAnalogSensors() {
	for (auto p : analogPins) {
		AnalogSensor s(p);
		if (s.sensorPresent()) sensors.push_back((Sensor) s);
	}
}

void EcoSensors::attachSDISensors() {

	// TODO: check all pins that are not allready used by analog as well?

	for (auto p : sdiPins) {
		SDISensor s(p);
		s.init();
		if (s.sensorPresent()) sensors.push_back((Sensor) s);
	}
}
