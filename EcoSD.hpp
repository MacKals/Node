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

    const String filename = "data_node";
    const String extension = ".txt";
    int fileNumber = 0;

    const String cachFile = "dataCach.txt";
public:

    EcoSD(int chipSelect = BUILTIN_SDCARD) : chipSelect(chipSelect) {}

    bool init() {

        if (SD.begin(chipSelect)) {
            initialized = true;
            PRINTLN("SD card initialized");
            return true;
        }

        initialized = false;
        PRINTLN("SD card failed or not present");
        return false;
    }

    bool writeDataToFile(String dataString) {

        if (initialized) {
            // open the file. note that only one file can be open at a time,
            // so you have to close this one before opening another.
            File dataFile = SD.open(fullFileName().c_str(), FILE_WRITE);

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

    String fullFileName() {
        String number = (fileNumber >= 10 ? "0" : "") + String(fileNumber);
        return filename + number + extension;
    }

    void updateFileNameInNeeded() {
        if (initialized) {
            File dataFile = SD.open(fullFileName().c_str());
            if (dataFile.size() > MAX_FILE_SIZE) fileNumber++;
        }
    }

    // ---- Buffering ----

    bool cachData(String data) {
        File file = SD.open(cachFile.c_str(), FILE_WRITE);
        if (file) {
            file.println(data);
            file.close();
            return true;
        }
        return false;
    }

    String popData() {

    }

    bool cashedData() {
        File file = SD.open(cachFile.c_str(), FILE_WRITE);
        if (file) {
            PRINTLN(file.size());
        }
        return false;
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
