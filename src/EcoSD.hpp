//  EcoSD.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-28


#ifndef _ECOSD_H
#define _ECOSD_H

#include <SD.h>
#include "Configuration.hpp"

class EcoSD {
private:
    const int chipSelect;
    bool initialized = false;

    const String extension = ".txt";
    const String cachDirectory = "cach";
    int32_t cachNumber = 0;

public:

    EcoSD(int chipSelect = BUILTIN_SDCARD) : chipSelect(chipSelect) {}

    bool init();

    // ---- Buffering ----
private:
    String cachFileName();
public:
    bool cachData(String data);
    String popData();
    bool cachedData();

    String getDataFromFile(String filename);

};


#endif
