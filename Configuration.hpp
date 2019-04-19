//  Configuration.hpp
//  UBC Ecohydrology Node
//
// Teensy 3.5 Pin mappings for RF95 LoRa module and general I/O pins
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _CONFIG_H
#define _CONFIG_H

#define DEBUG // comment to stop serial printing

#ifdef DEBUG
 #define PRINT(x)  Serial.println (x)
#else
 #define PRINT(x)
#endif


// ---- RF95 ----

#define RF95_FREQ 915.0 // MHz, transmission frequency

// Pin locations
#define RFM95_CS  10 // slave select
#define RFM95_RST 24 // reset
#define RFM95_INT 28 // interupt
#define RFM95_CLK 14 // clock, not primary

#define TIMEOUT_WAIT_TIME 3000 // ms

// ---- SDI-12 ----
#define SDI12_DATA_PIN 33
#define SDI12_BAUD 115200   // The baud rate for the output serial port

// Analog Pins

// Digital Pins
#define LED 13

//Address Pins
#define DIP0 4
#define DIP1 5
#define DIP2 6
#define DIP3 7
#define DIP4 8
#define DIP5 9

#endif
