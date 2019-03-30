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
#define RFM95_INT 25 // interupt
#define RFM95_CLK 14 // clock, not primary

#include <RH_RF95.h>
#include <SPI.h>

class EcoRadio {
    RH_RF95 lora = RH_RF95(RFM95_CS, RFM95_INT);

public:
    EcoRadio() {
        Serial.println("started radio");
    }

    bool init() {
        SPI.setSCK(RFM95_CLK);

        lora.init();
        if (!lora.init()) Serial.println("unable to init lora");
        Serial.println("lora init ok");
        lora.setFrequency(RF95_FREQ);
    }

    void printRegisters() {
        lora.printRegisters();
    }

    bool recievedData();
    String readData();

    bool sendData(String data);

};

#endif
