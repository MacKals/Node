//  Configuration.hpp
//  UBC Ecohydrology Node
//
// Teensy 3.5 Pin mappings for RF95 LoRa module and general I/O pins
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _CONFIG_H
#define _CONFIG_H


// ---- DEBUG ----

#define DEBUG // comment out to deactivate serial printing

#ifdef DEBUG
 #define PRINT(x)      Serial.print(x)
 #define PRINTLN(x)    Serial.println(x)
 #define PRINT2(x,y)   Serial.print(x,y)
 #define PRINTLN2(x,y) Serial.println(x,y)
#else
 #define PRINT(x)
 #define PRINTLN(x)
 #define PRINT2(x,y)
 #define PRINTLN2(x,y)
#endif


// ---- POWER SAVE ----
// Methods and calls to remove when wanting to conserve power
// but that might have to make operator more carefull.
// Limitations:
// - all sensors must be connected when system power up
//

#define POWER_SAVE

#define RECORD_INTERVAL   20 // s, time between data-collections
#define TRANSMIT_INTERVAL 10 // s, time between transmit attempts


// ---- EEPROM ----
#define EEPROM_INIT_ADDRESS 100 // byte to check if system has been initialized before
#define EEPROM_INIT_VAL 88      // a number between 0 and 255

#define EEPROM_BOOTCOUNT_MSB_ADDRESS 101 // byte to check boot count, most significant
#define EEPROM_BOOTCOUNT_LSB_ADDRESS 102 // byte to check boot count, least significant


// ---- FILES ----
#define LORAWAN_CONFIG_FILE_NAME "lorawan.txt"
#define SENSORS_CONFIG_FILE_NAME "sensors.txt"

#define MAX_FILE_SIZE 16000000 // bytes, max size of files on SD card

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

// SD card
#define SDCARD BUILTIN_SDCARD

#endif
