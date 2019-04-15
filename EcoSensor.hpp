//  EcoSensor.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#ifndef _ECOSENSOR_H
#define _ECOSENSOR_H

#include "Configuration.hpp"
#include "Libraries/Arduino-SDI-12-Teensy-35/src/SDI12_Teensy35.h"
#include <list>

class EcoSensor {

    // Define the SDI-12 bus
    // SDI12 sdi12 = SDI12(SDI12_DATA_PIN);

	std::list<byte> sdi12_addr;

    // Methods from example library
    // byte charToDec(char i);
    // char decToChar(byte i);
    // void printBufferToScreen();
    // void printInfo(char i);
    // void takeMeasurement(char i);
    // bool checkActive(char i);
    // bool isTaken(byte i);
    // bool setTaken(byte i);
    // bool setVacant(byte i);

public:


	void attachAllSDI12() {
		// for(byte i = '0'; i <= '9'; i++) if(checkActive(i)) {sdi12_addr.push_back(i);}   // scan address space 0-9
		// for(byte i = 'a'; i <= 'z'; i++) if(checkActive(i)) {sdi12_addr.push_back(i);}   // scan address space a-z
		// for(byte i = 'A'; i <= 'Z'; i++) if(checkActive(i)) {sdi12_addr.push_back(i);}   // scan address space A-Z
		// Serial.println("Found " + String(sdi12_addr.size()));
		// for (auto const& value: sdi12_addr) {
		// 	Serial.println(value);
		// }
		Serial.println("Yeah");
	}




    // String getData() {
	//
    // }

    };

#endif
