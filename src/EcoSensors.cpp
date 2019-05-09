//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"


void EcoSensors::init() {
	attachPWMSensors();
	attachSDISensors();
	// attachFlowSensors();
	// attachAnalogSensors();

	PRINT("Connected sensors: ");
	for (auto s = this->sensors.begin(); s != this->sensors.end(); ++s) {
		PRINT((*s)->address);
		PRINT(" ");
	}
	PRINTLN("");
}

bool EcoSensors::initSensorFromString(String s) {

	PRINTLN("init from this: " + s);

	// get all parameters in array

	// assign

	// switch (s.charAt(0)) {
	// 	case 'A': analogSensorFromString(); break;
	// 	case 'S': ; break;
	// 	case 'P': ; break;
	// 	case 'F': ; break;
	// 	default:
	// 		PRINTLN("Invalid sensor spesified, does not recognize first character.");
	// }
}


// reads data from all sensors and places them in string
// Format:
// - values from different sensors separated by &
// - starts with unix time-stamp
// - data from all sensors follow in this format:
//   - pinNumber:rading1,reading2,reading3 (etc for more parameters)
// example: 1557236143&3:100000.0,21.8&17:1.27,22.8,1&22:1.39,22.0,1&2:2,2194
String EcoSensors::getFullDataString() {

	time_t t = Teensy3Clock.get(); // s, time since 1.1.1970 (unix time)
	String data = String(t);

	// loop through sensors using itterators
	for (auto s = sensors.begin(); s != sensors.end(); ++s) {
		data += "&" + String((*s)->address) + ":";  // sensor address
		data += (*s)->readDataToString();			// append data
	}
	return data;
}

// Check if there is sensor of relevant type on pin and add it to array of sensors if so.
bool EcoSensors::attachSensorIfPresent(Sensor * s) {
	if (s->sensorPresent()) {
		PRINT("pin " + String(s->pin) + " ok \t");
		sensors.push_back(s);
		return true;
	}

	// sensor not present
	PRINT("pin " + String(s->pin) + " -  \t");
	return false;
}

bool EcoSensors::pinInUse(uint8_t pin) {
	for (auto s : sensors) {
		if (s->pin == pin) return true;
	}
	return false;
}


void EcoSensors::attachAnalogSensors() {
	PRINT("Attaching analog sensors:\t");
	for (const uint8_t &p : threePortPins) {
		if (!pinInUse(p)) {
			AnalogSensor *s = new AnalogSensor(p);
			if (!attachSensorIfPresent(s)) delete s;
		}
	}
	PRINTLN();
}

void EcoSensors::attachPWMSensors() {
	PRINT("Attaching PWM sensors:\t\t");
	for (const uint8_t &p : threePortPins) {
		if (!pinInUse(p)) {
			PWMSensor *s = new PWMSensor(p);
			if (!attachSensorIfPresent(s)) delete s;
		}
	}
	PRINTLN();
}

void EcoSensors::attachSDISensors() {
	PRINT("Attaching SDI sensors:\t\t");
	for (auto p : threePortPins) {
		if (!pinInUse(p)) {
			SDISensor *s = new SDISensor(p);
			s->init();
			if (!attachSensorIfPresent(s)) {
				s->end(); // free resources
				delete s;
			}
		}
	}
	PRINTLN();
}

void EcoSensors::attachFlowSensors() {
	PRINT("Attaching flow sensors:\t\t");
	for (auto p : threePortPins) {
		if (!pinInUse(p)) {
			FlowSensor *s = new FlowSensor(p);
			if (attachSensorIfPresent(s)) s->init(); // activate interrupt
			else delete s;
		}
	}
	PRINTLN();
}
