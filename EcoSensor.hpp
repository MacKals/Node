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

    // Methods from example library
    // byte charToDec(char i);
    // char decToChar(byte i);
    void printBufferToScreen();
    // void printInfo(char i);
    // void takeMeasurement(char i);
    bool checkActive(char i);
    // bool isTaken(byte i);
    // bool setTaken(byte i);
    // bool setVacant(byte i);

public:


	void attachAllSDI12() {
		Serial.println("in");
		for (byte i = '0'; i <= '9'; i++) {
			if (this->checkActive(i)) {
				this->sdi12_addr += (char) i;
			}   // scan address space 0-9
		}
		// for(byte i = 'a'; i <= 'z'; i++) if(this->checkActive(i)) {this->sdi12_addr.push_back(i);}   // scan address space a-z
		// for(byte i = 'A'; i <= 'Z'; i++) if(this->checkActive(i)) {this->sdi12_addr.push_back(i);}   // scan address space A-Z
		Serial.println("Found " + String(sdi12_addr.length()) + " at " + sdi12_addr);
	}




    // String getData() {
	//
    // }

    };

#endif
