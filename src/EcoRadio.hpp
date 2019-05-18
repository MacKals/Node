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
#include "EcoSD.hpp"

class EcoRadio {
private:
	void setLoRaParameters(String deveui, String appeui, String appkey);

public:
	void init(EcoSD sd);

	void loop() {
		os_runloop_once(); // queries replies from LORA module, must keep runing
	}

	bool send(String s);
	bool ready();

	bool transmitting();
	bool transmitSuccessfull();
};

#endif
