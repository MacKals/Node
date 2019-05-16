//  Radio.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

// Adafruit totorial
// https://learn.adafruit.com/the-things-network-for-feather?view=all

#ifndef _ECORADIO_H
#define _ECORADIO_H


#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

#include "Configuration.hpp"


class EcoRadio {

public:

	void setLoRaParameters(String deveui, String appeui, String appkey);

	void init() {
		SPI.setSCK(RFM95_CLK); // update clock pin

		// LMIC init.
	    os_init();
	    // Reset the MAC state. Session and pending data transfers will be discarded.
	    LMIC_reset();
	    // Disable link-check mode and ADR, because ADR tends to complicate testing.
	    LMIC_setLinkCheckMode(0);
	    // Set the data rate to Spreading Factor 7.  This is the fastest supported rate for 125 kHz channels, and it
	    // minimizes air time and battery power. Set the transmission power to 14 dBi (25 mW).
	    LMIC_setDrTxpow(DR_SF7,14);
	    // in the US, with TTN, it saves join time if we start on subband 1 (channels 8-15). This will
	    // get overridden after the join by parameters from the network. If working with other
	    // networks or in other regions, this will need to be changed.
	    LMIC_selectSubBand(1);
	}

	void loop() {
		// queries replies from LORA module
		// must keep runing
		os_runloop_once();
	}

	void send(String s);
	bool ready();

	bool sendSucceeded();
};

#endif
