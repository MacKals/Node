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

	void do_send(osjob_t* j);


public:
	EcoRadio() {
		Serial.println("radio object created");
	}

	bool init() {
		SPI.setSCK(RFM95_CLK); // update clock pin

		os_init();    // LMIC init
		LMIC_reset(); // Reset the MAC state. Session and pending data transfers will be discarded.

		do_send(&sendjob); // Start job (sending automatically starts OTAA too)
	}

};

#endif
