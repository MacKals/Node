//  EcoSensor.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#ifndef _ECOSENSOR_H
#define _ECOSENSOR_H

#include "Configuration.hpp"
#include "Libraries/Arduino-SDI-12-Teensy-35/src/SDI12_Teensy35.h"

class EcoSensor {

    // Define the SDI-12 bus
    SDI12 sdi12 = SDI12(SDI12_DATA_PIN);

    // keeps track of active addresses
    // each bit represents an address:
    // 1 is active (taken), 0 is inactive (available)
    // setTaken('A') will set the proper bit for sensor 'A'
	byte addressRegister[8] = {
	  0B00000000,
	  0B00000000,
	  0B00000000,
	  0B00000000,
	  0B00000000,
	  0B00000000,
	  0B00000000,
	  0B00000000
	};

	String sdi12_addr = "";
    uint8_t numSensors = 0;


    // Methods from example library
    byte charToDec(char i);
    char decToChar(byte i);
    void printBufferToScreen();
    void printInfo(char i);
    void takeMeasurement(char i);
    bool checkActive(char i);
    bool isTaken(byte i);
    bool setTaken(byte i);
    bool setVacant(byte i);

public:

	void attachAllSensors() {
		for(byte i = '0'; i <= '9'; i++) if(checkActive(i)) {numSensors++; sdi12_addr += i;}   // scan address space 0-9
		for(byte i = 'a'; i <= 'z'; i++) if(checkActive(i)) {numSensors++; sdi12_addr += i;}   // scan address space a-z
		for(byte i = 'A'; i <= 'Z'; i++) if(checkActive(i)) {numSensors++; sdi12_addr += i;}   // scan address space A-Z
		Serial.println("Found " + String(numSensors) + " sensors at " + sdi12_addr);
	}



    String getData() {

    }

    };

#endif
