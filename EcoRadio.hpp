//  Radio.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECORADIO_H
#define _ECORADIO_H


#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

#include "Configuration.hpp"


class EcoRadio {

public:
	EcoRadio() {
		Serial.println("radio object created");
	}

	void init() {
		SPI.setSCK(RFM95_CLK); // update clock pin

		os_init();    // LMIC init
		LMIC_reset(); // Reset the MAC state. Session and pending data transfers will be discarded.
	}

	void loop() {
		// queries replies from LORA module
		// must keep runing
		os_runloop_once();
	}

	void send(String s);

};

#endif
