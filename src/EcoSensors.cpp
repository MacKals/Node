//  EcoSensors.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensors.hpp"


void EcoSensors::init(EcoSD sd) {

    // set all power contol pins to output and turn on
    for (auto & p : powerControlPins) {
        pinMode(p, OUTPUT);
        digitalWrite(p, HIGH);
    }

    vector<vector<String>> connected = sd.getSensorsFromConfig(threePortPins);

	for (auto & sensor : connected) {

		switch (sensor[1].charAt(0)) {
			case 'S': case 's':
                {
                    SDISensor *s = new SDISensor(sensor[0].toInt(), sensor[2]);
                    s->init();
				    sensors.push_back(s);
				    break;
                }
			case 'A': case 'a':
				sensors.push_back(new AnalogSensor(sensor[0].toInt(), sensor[2]));
				break;
			case 'F': case 'f':
				sensors.push_back(new FlowSensor(sensor[0].toInt(), sensor[2]));
				break;
			case 'P': case 'p':
				sensors.push_back(new PWMSensor(sensor[0].toInt(), sensor[2]));
				break;
			default:
			PRINTLN("Couldn't match sensor type string from config to a valid type.");
		}
	}

    // power off sensors
    for (auto & p : powerControlPins) digitalWrite(p, LOW);

	PRINT("Connected sensors: ");
	for (auto s = this->sensors.begin(); s != this->sensors.end(); ++s) {
		PRINT((*s)->pin);
		PRINT(" ");
	}
	PRINTLN("");
}

// powers sensors on if on switched power-rail, nothing otherwise
void EcoSensors::powerOn(uint8_t pin) {
    for (uint8_t i = 0; i < poweredPins.size(); i++) {
        if (poweredPins[i] == pin) {
            digitalWrite(powerControlPins[i], HIGH);
            delay(40);
        }
    }
}

void EcoSensors::powerOff(uint8_t pin) {
    for (uint8_t i = 0; i < poweredPins.size(); i++) {
        if (poweredPins[i] == pin) {
            digitalWrite(powerControlPins[i], LOW);
        }
    }
}


// reads data from all sensors and places them in string
// Format:
// - values from different sensors separated by &
// - starts with unix time-stamp
// - data from all sensors follow in this format:
//   - pinNumber:reading1,reading2,reading3 (etc for more parameters)
// example: 1557236143&3:100000.0,21.8&17:1.27,22.8,1&22:1.39,22.0,1&2:2,2194
String EcoSensors::getFullDataString() {
	String data = "";

	// loop through sensors using iterators
	for (auto s = sensors.begin(); s != sensors.end(); ++s) {
        powerOn((*s)->pin); // power on and delay if on powered domain
		data += "&" + String((*s)->pin) + ":";  // sensor address
		data += (*s)->readDataToString();		// append data
        powerOff((*s)->pin); // power off
	}
	return data;
}

String EcoSensors::getFullConfiguration() {
	String config = "";

	// loop through sensors using itterators
	for (auto s = sensors.begin(); s != sensors.end(); ++s) {
		config += "&" + String((*s)->pin) + ":";  // sensor address
		config += String((*s)->serialNum);        // sensor serial number
	}
	return config;
}


bool EcoSensors::pinInUse(uint8_t pin) {
	for (auto s : sensors) {
		if (s->pin == pin) return true;
	}
	return false;
}


// void EcoSensors::attachAnalogSensors() {
// 	PRINT("Attaching analog sensors:\t");
// 	for (const uint8_t &p : threePortPins) {
// 		if (!pinInUse(p)) {
// 			AnalogSensor *s = new AnalogSensor(p);
// 			if (!attachSensorIfPresent(s)) delete s;
// 		}
// 	}
// 	PRINTLN();
// }
//
// void EcoSensors::attachPWMSensors() {
// 	PRINT("Attaching PWM sensors:\t\t");
// 	for (const uint8_t &p : threePortPins) {
// 		if (!pinInUse(p)) {
// 			PWMSensor *s = new PWMSensor(p);
// 			if (!attachSensorIfPresent(s)) delete s;
// 		}
// 	}
// 	PRINTLN();
// }
//
// void EcoSensors::attachSDISensors() {
// 	PRINT("Attaching SDI sensors:\t\t");
// 	for (auto p : threePortPins) {
// 		if (!pinInUse(p)) {
// 			SDISensor *s = new SDISensor(p);
// 			s->init();
// 			if (!attachSensorIfPresent(s)) {
// 				s->end(); // free resources
// 				delete s;
// 			}
// 		}
// 	}
// 	PRINTLN();
// }
//
// void EcoSensors::attachFlowSensors() {
// 	PRINT("Attaching flow sensors:\t\t");
// 	for (auto p : threePortPins) {
// 		if (!pinInUse(p)) {
// 			FlowSensor *s = new FlowSensor(p);
// 			if (attachSensorIfPresent(s)) s->init(); // activate interrupt
// 			else delete s;
// 		}
// 	}
// 	PRINTLN();
// }
