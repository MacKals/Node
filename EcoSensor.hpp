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

class EcoSensor {

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
		PRINT("Init sensor class.");

		this->sdi12.begin();
		delay(500);

		this->attachAllSDI12();

		if (this->addressAttached('B')) {
			this->changeAddress('B', 'A');
		}
	}

	void attachAllSDI12() {
		PRINT("Attaching all SDI-12 nodes.");

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

#endif
