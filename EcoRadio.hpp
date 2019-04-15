//  Radio.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECORADIO_H
#define _ECORADIO_H

#include "Libraries/RadioHead/RH_RF95.h"
#include "Libraries/SPI/SPI.h"
#include "Configuration.hpp"

/*
    Defining header format:
    first octet: 76543210
     0 = reciept (1 = yes)
     1 = 0
     2 = 0
     3 = 0
     4 = 0
     5 = 0
     6 = 0
     7 = 0
    second octet: sender address
    third octet: recipient address (undefined gives all zeros)
    4,5,6 and 7th octet: timestamp, 32 bit
    8 to 251: data
 */


class EcoRadio {
RH_RF95 lora = RH_RF95(RFM95_CS, RFM95_INT);


// Max number of octets the LORA Rx/Tx FIFO can hold is 255
// header is 4 octets

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void sendRawData(String data) {
	const uint8_t* d = reinterpret_cast<const uint8_t*>(&data[0]);
	lora.send(d, sizeof(d));

	// TODO: keep? test transmit time?
	lora.waitPacketSent();
}


public:
EcoRadio() {
	Serial.println("radio object created");
}

bool init() {
	SPI.setSCK(RFM95_CLK);

	lora.init();
	if (!lora.init()) Serial.println("unable to init lora");
	Serial.println("lora init ok");
	lora.setFrequency(RF95_FREQ);

	// find parent? Send message telling everybody you are here.
}

void printRegisters() {
	lora.printRegisters();
}

bool available() {
	return lora.available();
}

String readData() {
	lora.recv(buf, &len);

	// send reciept?
	return String((char*) buf);
}

bool sendDataWithReciept(String data, time_t time, uint8_t from, uint8_t to = 0);

uint8_t lastSNR() {
	return lora.lastSNR();
}

uint8_t getParentAddress() {
	return 0;
}

};

#endif
