//  EcoSD.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-28


#ifndef _ECOSD_H
#define _ECOSD_H

#include <SPI.h>
#include <SD.h>
#include "Configuration.hpp"

class EcoSD {
private:
    const int chipSelect;
    bool initialized = false;

    String filename = "UBC_ecohydro_datalog.txt";

public:

    EcoSD(int chipSelect = BUILTIN_SDCARD) : chipSelect(chipSelect) {}

    void init() {
        if (SD.begin(chipSelect)) {
            initialized = true;
            PRINTLN("SD card initialized");
        } else {
            initialized = false;
            PRINTLN("SD card failed or not present");
        }
    }

    bool writeDataToFile(String dataString) {
        if (initialized) {

            // open the file. note that only one file can be open at a time,
            // so you have to close this one before opening another.
            File dataFile = SD.open(filename.c_str(), FILE_WRITE);

            // if the file is available, write to it:
            if (dataFile) {
                dataFile.println(dataString);
                dataFile.close();
                PRINTLN("SD write successfull");
                return true;
            }
        }

        PRINTLN("Error printing to SD file");
        return false;
    }

};


#endif
