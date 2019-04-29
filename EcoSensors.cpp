//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"

void EcoSensors::attachAnalogSensors() {
	for (const uint8_t &p : this->analogPins) {
		AnalogSensor *s = new AnalogSensor(p);
		if (s->sensorPresent()) this->sensors.push_back(s);
	}
}

void EcoSensors::attachSDISensors() {
	// TODO: check all pins that are not allready used by analog as well?
	for (auto p : sdiPins) {
		SDISensor *s = new SDISensor(p);
		s->init();

		if (s->sensorPresent()) {
			this->sensors.push_back(s);
			s->printInfo();
		} else {
			s->end();
			delete s;
		}
	}
}

void EcoSensors::attachFlowSensors() {
	// TODO: check all pins that are not allready used?
	for (auto p : flowPins) {
		// TODO: attach interrupt?
		FlowSensor *s = new FlowSensor(p);
		if (s->sensorPresent()) this->sensors.push_back(s);
	}
}
