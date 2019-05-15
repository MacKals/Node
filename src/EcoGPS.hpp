//  EcoGPS.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-05-08
//
// UBLOX library:

#ifndef _ECOGPS_H
#define _ECOGPS_H

#include "Configuration.hpp"
#include <Arduino.h>
//#include <UBLOX.h>

class EcoGPS {

private:
    //UBLOX *gps;

    uint8_t RST_n = 3;
    uint8_t INT = 5;

public:
    void init() {

        pinMode(RST_n, OUTPUT);
        digitalWrite(RST_n, HIGH);

        pinMode(INT, INPUT);

        //gps = new UBLOX(Serial1, 115200);
        //gps->begin();
    }

    void hybernate() {

    }

    void printData() {
      //  PRINTLN("GPS data from fix: " + String(gps->isGnssFixOk()));

        // checking to see if a good packet has
        // been received and displaying some
        // of the packet data
      //   if (gps->readSensor()) {
      //     PRINT(gps->getYear());                ///< [year], Year (UTC)
      //     PRINT("\t");
      //     PRINT(gps->getMonth());               ///< [month], Month, range 1..12 (UTC)
      //     PRINT("\t");
      //     PRINT(gps->getDay());                 ///< [day], Day of month, range 1..31 (UTC)
      //     PRINT("\t");
      //     PRINT(gps->getHour());                ///< [hour], Hour of day, range 0..23 (UTC)
      //     PRINT("\t");
      //     PRINT(gps->getMin());                 ///< [min], Minute of hour, range 0..59 (UTC)
      //     PRINT("\t");
      //     PRINT(gps->getSec());                 ///< [s], Seconds of minute, range 0..60 (UTC)
      //     PRINT("\t");
      //     PRINT(gps->getNumSatellites());       ///< [ND], Number of satellites used in Nav Solution
      //     PRINT("\t");
      //     PRINT2(gps->getLatitude_deg(),10);     ///< [deg], Latitude
      //     PRINT("\t");
      //     PRINT2(gps->getLongitude_deg(),10);    ///< [deg], Longitude
      //     PRINT("\t");
      //     PRINTLN(gps->getMSLHeight_ft());      ///< [ft], Height above mean sea level
      // } else {
      //     PRINTLN("GPS not ready");
      // }
    }
};

#endif
