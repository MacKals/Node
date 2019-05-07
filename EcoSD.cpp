
#include "EcoSD.hpp"


bool EcoSD::init() {

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

String EcoSD::cachFileName() {
    char n[100] = {};
    sprintf(n, "%08d", this->cachNumber);
    return cachDirectory + "/" + String(n) + this->extension;
}

bool EcoSD::cachData(String data) {
    this->cachNumber++;
    File file = SD.open(cachFileName().c_str(), FILE_WRITE);
    PRINTLN("Caching data to file\n" + cachFileName() + " \t");
    if (file) {
        file.print(data);
        file.close();

        return true;
    }
    PRINTLN("Failed to cach data");
    return false;
}

String EcoSD::popData() {

    if (!cachedData()) return "";

    File file = SD.open(cachFileName().c_str());
    if (file) {
        String ret = "";
        while (file.available()) {
            char c = file.read();
            ret += c;
        }
        ret.trim(); // remove ending newline
        file.close();

        bool removed = SD.remove(cachFileName().c_str());
        if (!removed) PRINTLN("Delete failed");

        cachNumber--;
        return ret;
    }

    PRINTLN("popData failed");
    return "";
}

bool EcoSD::cachedData() {
    return cachNumber >= 0;
}


// ---- Config ----

// return as one string, convert one level up
String EcoSD::getConfigData() {
    File file = SD.open(CONFIG_FILE_NAME);
    if (file) {

        String ret = "";
        while (file.available()) ret += file.read();
        file.close();

        return ret;
    }
    return "";
}
