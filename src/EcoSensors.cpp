//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"


void EcoSensors::init() {
	// attachPWMSensors();
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


bool EcoSensors::initSensorFromString(String inputString) {
	String input = inputString.trim();
	PRINTLN("init from this: " + input);

	// split string into vector of parameters
	std::vector<String> parameters;
	uint16_t startIndex = 0;
	for (uint16_t i = startIndex; i < input.length(); i++) {
		if (input.charAt(i) == ',') {
			parameters.push_back(input.substring(startIndex,i).trim());
			startIndex = i+1;
		}
	}
	// handle last part of string (if present)
	if (startIndex < input.length()) {
		parameters.push_back(input.substring(startIndex).trim());
	}

	// determine sensor identity and initialize relevant sensor if parameter count is right
	switch (input.charAt(0)) {

		case 'A':
			if (parameters.size() == 7) {
				Sensor *s = new AnalogSensor(parameters[1].toInt(),
											 parameters[2].toFloat(),
											 parameters[3].toFloat(),
											 parameters[4].toFloat(),
											 parameters[5].toFloat(),
											 parameters[6]);
				sensors.push_back(s);
				return;
			} else PRINTLN("Wrong number of parameters for analog sensor: is " + String(parameters.size()) + ", expecting 7.");

		case 'S':
			if (parameters.size() > 2) {
				String titles = "";
				for (uint8_t i = 2; i < parameters.size(); i++) titles += parameters[i] + ",";
				Sensor *s = new SDISensor(parameters[1].toInt(), titles);
				sensors.push_back(s);
				return;
			} else PRINTLN("Wrong number of parameters for SDI sensor: is " + String(parameters.size()) + ", must exceed 2.");

		// case 'P': ; break;
		// case 'F': ; break;
		default:
			PRINTLN("Invalid sensor spesified, does not recognize first character in: " + input);
	}
}


// reads data from all sensors and places them in string
// Format:
// - values from different sensors separated by &
// - starts with unix time-stamp
// - data from all sensors follow in this format:
//   - pinNumber:rading1,reading2,reading3 (etc for more parameters)
// example: 1557236143&3:100000.0,21.8&17:1.27,22.8,1&22:1.39,22.0,1&2:2,2194
String EcoSensors::getFullDataString() {
	String data = "";

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
