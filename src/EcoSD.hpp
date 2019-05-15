//  EcoSD.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-28


#ifndef _ECOSD_H
#define _ECOSD_H

#include <SD.h>
#include "Configuration.hpp"
#include <IniFile.h>
#include <vector>
#include <tuple>
using namespace std;

class EcoSD {
private:
    const int chipSelect;
    bool initialized = false;
    IniFile ini = IniFile("config.ini");
    const String extension = ".txt";
    const String cachDirectory = "cach";
    uint16_t cachNumber = 0;

public:

    EcoSD(int chipSelect = SDCARD) : chipSelect(chipSelect) {}

    bool init();

    // ---- Buffering ----
    String cachFileName();
    bool cachData(String data);
    String popData();
    bool cachedData();
    String getDataFromFile(String filename);
    vector<vector<String>> getSensorsFromConfig(const vector<uint8_t> validPins);

};


#endif
