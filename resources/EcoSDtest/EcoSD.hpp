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


    const String cachDirectory = "cach";
    int32_t cachNumber = 0;

    const String extension = ".txt";

public:

    EcoSD(int chipSelect = BUILTIN_SDCARD) : chipSelect(chipSelect) {}

    bool init() {

        if (SD.begin(chipSelect)) {
            initialized = true;
            PRINTLN("SD card initialized");

            if (!SD.exists(cachDirectory.c_str())) {
                SD.mkdir(cachDirectory.c_str());
            }

            while (SD.exists(cachFileName().c_str())) {
                cachNumber++;
            }
            cachNumber--;

            return true;
        }

        initialized = false;
        PRINTLN("SD card failed or not present");
        return false;

    }


    // ---- Buffering ----

    String cachFileName() {
        char n[100] = {};
        sprintf(n, "%08d", this->cachNumber);
        return cachDirectory + "/" + String(n) + this->extension;
    }

    bool cachData(String data) {
        this->cachNumber++;
        File file = SD.open(cachFileName().c_str(), FILE_WRITE);
        PRINT(cachFileName() + " \t");
        if (file) {
            file.print(data);
            file.close();

            return true;
        }
        PRINTLN("Fail");
        return false;
    }

    String popData() {

        if (!cachedData()) return "";

        File file = SD.open(cachFileName().c_str());
        if (file) {
            String ret = "";
            while (file.available()) {
                char c = file.read();
                ret += c;
            }
            ret.remove(ret.length()); // remove ending newline
            file.close();

            bool removed = SD.remove(cachFileName().c_str());
            if (!removed) PRINTLN("Delete failed");

            cachNumber--;
            return ret;
        }

        PRINTLN("popData failed");
        return "";
    }

    bool cachedData() {
        return cachNumber >= 0;
    }




    // return data line from file and delete
    String getDataToSend() {
        File file = SD.open(CONFIG_FILE_NAME, FILE_WRITE);
        if (file) {

            String ret = "";
            while (file.available()) {
                ret += file.read();
            }
            file.close();

            return ret;
        }
        return "";
    }

    // ---- Config ----

    // return as one string, convert one level up
    String getConfigData() {
        File file = SD.open(CONFIG_FILE_NAME);
        if (file) {

            String ret = "";
            while (file.available()) ret += file.read();
            file.close();

            return ret;
        }
        return "";
    }

};


#endif
