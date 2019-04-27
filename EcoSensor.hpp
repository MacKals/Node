//  EcoSensor.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#ifndef _ECOSENSOR_H
#define _ECOSENSOR_H

#include "Configuration.hpp"
#include <SDI12_Teensy35.h>
//#include "Libraries/Arduino-SDI-12-Teensy-35/src/SDI12_Teensy35.h"

enum SensorType {
    Serial12, //SDI-12
    Digital,
    Analog,
    Differential,
};

class EcoSensor {

  SensorType sensorType;
  uint8_t pin1;
  uint8_t pin2; // differential signals

public:

  SensorType getSensorType();
  double read();

private:
    // Define the SDI-12 bus
    SDI12 sdi12 = SDI12(SDI12_DATA_PIN);

	String sdi12_addr = "";

    String printBufferToString();
    void printInfo(char i);
    String takeMeasurement(char i);
    bool checkActive(char i);
	bool changeAddress(char from, char to);
	bool addressAttached(char i);
public:

	void init() {
		PRINTLN("Init sensor class.");

		this->sdi12.begin();
		delay(500);

		this->attachAllSDI12();

		if (this->addressAttached('B')) {
			this->changeAddress('B', 'A');
		}
	}

	void attachAllSDI12() {
		PRINTLN("Attaching all SDI-12 nodes.");

		// clear existing
		sdi12_addr = "";

		// serach through all valid addresses
		for (byte i = '0'; i <= '9'; i++) if (this->checkActive(i)) {this->sdi12_addr += (char) i;}   // scan address space 0-9
		for (byte i = 'a'; i <= 'z'; i++) if (this->checkActive(i)) {this->sdi12_addr += (char) i;}   // scan address space a-z
		for (byte i = 'A'; i <= 'Z'; i++) if (this->checkActive(i)) {this->sdi12_addr += (char) i;}   // scan address space A-Z

		Serial.println("Found " + String(sdi12_addr.length()) + " at " + sdi12_addr);
	}

    String readAllSensors() {
		String data = "";
		for (uint8_t i = 0; i < sdi12_addr.length(); i++) {
			data += takeMeasurement(sdi12_addr.charAt(i));
		}
		return data;
    }

};

class Sensor {
protected:
    char address; // sensor address

public:
    // Return address of sensor as a character
    char getAddress() {
        return address;
    }

    void EcoSensor(char address) {
        this->address = address;
    }

    // return data read by sensor as a string. Start with address followed b
    // by comma and information
    virtual String readDataToString();
};


class AnalogSensor : public Sensor {



};

#endif
