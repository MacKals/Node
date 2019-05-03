//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"

void EcoSensors::attachAnalogSensors() {
	for (const uint8_t &p : analogPins) {
		AnalogSensor *s = new AnalogSensor(p);
		if (s->sensorPresent()) sensors.push_back(s);
	}
}

void EcoSensors::attachPWMSensors() {
	for (const uint8_t &p : sdiPins) {
		PWMSensor *s = new PWMSensor(p);
		if (s->sensorPresent()) sensors.push_back(s);
	}
}

void EcoSensors::attachSDISensors() {
	// TODO: check all pins that are not allready used by analog as well?
	for (auto p : sdiPins) {
		SDISensor *s = new SDISensor(p);
		s->init();

		if (s->sensorPresent()) {
			this->sensors.push_back(s);
			PRINT("Pin " + String(p) + " found sensor \t");
			s->printInfo();
		} else {
			s->end();
			delete s;
			PRINTLN("Pin " + String(p) + " no sensor");
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
