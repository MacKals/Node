//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"

bool EcoSensors::attachSensorIfPresent(Sensor * s) {
	if (s->sensorPresent()) {
		PRINT("Pin " + String(s->pin) + " ok, \t");
		sensors.push_back(s);
		return true;
	}

	// sensor not present
	PRINT("Pin " + String(s->pin) + " -, \t");
	delete s;
	return false;
}

bool EcoSensors::pinInUse(uint8_t pin) {
	for (auto s : sensors) {
		if (s->pin == pin) return true;
	}
	return false;
}

void EcoSensors::attachAnalogSensors() {
	PRINT("Attaching analog sensors: ");
	for (const uint8_t &p : analogPins) {
		if (!pinInUse(p)) {
			AnalogSensor *s = new AnalogSensor(p);
			attachSensorIfPresent(s);
		}
	}
	PRINTLN();
}

void EcoSensors::attachPWMSensors() {
	PRINT("Attaching PWM sensors: ");
	for (const uint8_t &p : sdiPins) {
		if (!pinInUse(p)) {
			PWMSensor *s = new PWMSensor(p);
			attachSensorIfPresent(s);
		}
	}
	PRINTLN();
}

void EcoSensors::attachSDISensors() {
	// TODO: check all pins that are not allready used by analog as well?
	PRINT("Attaching SDI sensors: ");
	for (auto p : sdiPins) {
		if (!pinInUse(p)) {
			SDISensor *s = new SDISensor(p);
			s->init();
			if (!attachSensorIfPresent(s)) s->end();
		}
	}
	PRINTLN();
}

void EcoSensors::attachFlowSensors() {
	// TODO: check all pins that are not allready used?
	for (auto p : flowPins) {
		// TODO: attach interrupt?
		if (!pinInUse(p)) {
			FlowSensor *s = new FlowSensor(p);
			attachSensorIfPresent(s);
		}
	}
	PRINTLN();
}
