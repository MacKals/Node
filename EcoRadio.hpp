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

    String error = "";

public:
    EcoRadio() {
        SPI.setSCK(RFM95_CLK);

        if (!lora.init()) error = "unable to init lora";
        lora.setFrequency(RF95_FREQ);
    }

    bool recievedData();
    String readData();

    bool sendData(String data);

};

#endif
