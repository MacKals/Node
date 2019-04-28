//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"

void EcoSensors::attachAnalogSensors() {
	uint8_t num = sizeof(this->analogPins)/sizeof(this->analogPins[0]);

	for (uint8_t i = 0; i < num; i++) {
		uint8_t p = this->analogPins[i];
		AnalogSensor s(p);
		if (s.sensorPresent()) {
			Sensor s2 = (Sensor) s;
			this->sensors.push_back(s2);
		}
	}
	//
	// for (const uint8_t &p : this->analogPins) {
	// 	AnalogSensor s(p);
	// 	if (s.sensorPresent()) sensors.push_back((Sensor) s);
	// }
}

void EcoSensors::attachSDISensors() {

	// TODO: check all pins that are not allready used by analog as well?

	// for (auto p : sdiPins) {
	// 	SDISensor s(p);
	// 	s.init();
	// 	if (s.sensorPresent()) sensors.push_back((Sensor) s);
	// }
}
