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


    const String cacheDirectory = "cache";
    int32_t cacheNumber = 0;

    const String extension = ".txt";

public:

    EcoSD(int chipSelect = BUILTIN_SDCARD) : chipSelect(chipSelect) {}

    bool init() {

        if (SD.begin(chipSelect)) {
            initialized = true;
            PRINTLN("SD card initialized");

            if (!SD.exists(cacheDirectory.c_str())) {
                SD.mkdir(cacheDirectory.c_str());
            }

            while (SD.exists(cacheFileName().c_str())) {
                cacheNumber++;
            }
            cacheNumber--;

            return true;
        }

        initialized = false;
        PRINTLN("SD card failed or not present");
        return false;

    }


    // ---- Buffering ----

    String cacheFileName() {
        char n[100] = {};
        sprintf(n, "%08d", this->cacheNumber);
        return cacheDirectory + "/" + String(n) + this->extension;
    }

    bool cacheData(String data) {
        this->cacheNumber++;
        File file = SD.open(cacheFileName().c_str(), FILE_WRITE);
        PRINT(cacheFileName() + " \t");
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

        File file = SD.open(cacheFileName().c_str());
        if (file) {
            String ret = "";
            while (file.available()) {
                char c = file.read();
                ret += c;
            }
            ret.remove(ret.length()); // remove ending newline
            file.close();

            bool removed = SD.remove(cacheFileName().c_str());
            if (!removed) PRINTLN("Delete failed");

            cacheNumber--;
            return ret;
        }

        PRINTLN("popData failed");
        return "";
    }

    bool cachedData() {
        return cacheNumber >= 0;
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
