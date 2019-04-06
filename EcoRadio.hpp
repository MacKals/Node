//  Radio.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECORADIO_H
#define _ECORADIO_H

#define RF95_FREQ 915.0 // MHz, transmission frequency

// Pin locations
#define RFM95_CS  10 // slave select
#define RFM95_RST 24 // reset
#define RFM95_INT 28 // interupt
#define RFM95_CLK 14 // clock, not primary

#define TIMEOUT_WAIT_TIME 3000 // ms

#include <RH_RF95.h>
#include <SPI.h>

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

    // Max number of octets the LORA Rx/Tx FIFO can hold is 255
    // header is 4 octets

    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    String readData() {
        lora.recv(buf, &len);

        // send reciept?
        return str((char*) buf);
    }

    bool sendRawData(String data) {
        uint8_t d[] = data;
        rf95.send(d, sizeof(d));

        // TODO: keep? test transmit time?
        rf95.waitPacketSent();
    }

    bool sendDataWithReciept(String data, uint32_t time, uint8_t from, uint8_t to = 0) {

        uint8_t firstOctet = B10000000 | time % B100000; // expecting reciept
        Serial.print("First octet: ");
        Serial.println(firstOctet);

        sendRawData(data);
        if (lora.waitAvailableTimeout(TIMEOUT_WAIT_TIME)) {
            String d = readData();
            Serial.print("Recieved message=");
            Serial.println(d);
            // TODO: IS IT CORRECT? if (reciept = );
        }

        return false;
    }

    uint8_t lastSNR() {
        reutrn lora.lastSNR();
    }

    uint8_t getParentAddress() {
        return 0;
    }

};

#endif
