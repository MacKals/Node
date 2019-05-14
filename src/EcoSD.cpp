
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
    // return if SD card not initialized
    if (!initialized) return false;

    cachNumber++;
    File file = SD.open(cachFileName().c_str(), FILE_WRITE);
    if (file) {
        PRINTLN("Caching data to file " + cachFileName());
        file.print(data);
        file.close();
        return true;
    }
    // else
    PRINTLN("Failed to cach data");
    cachNumber--; // no need to increment, did not store data
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

    if (initialized) PRINTLN("popData failed");
    return "";
}

bool EcoSD::cachedData() {
    return cachNumber >= 0;
}


// ---- Config ----

// Returns all data in file.
// Empty string if file not found
String EcoSD::getDataFromFile(String filename) {
    File file = SD.open(filename.c_str());
    if (file) {
        String ret = "";
        while (file.available()) {
            char c = file.read();
            ret += c;
        }
        ret.trim(); // remove ending newline
        file.close();

        return ret;
    }

    if (initialized) PRINTLN("Could not find " + filename);
    return "";
}

Bool EcoSD::readFromConfig(int validPins[]) {

    const size_t bufferLen = 100;
    char buffer[bufferLen];

    if (!SD.begin(SDCARD)){
      PRINTLN("Could not find the SD card");
      return false;
    }

    IniFile ini(filename);

    if (!ini.open()) {
      PRINT("ini file ")
      PRINT(ini.getFilename());
      PRINTLN(" not valid.");
      return false;
    }

    PRINTLN("Ini file exists, reading values...");

    String entries[2] = {"Type", "SN"};

    for (i = 0; i < validPins.size()-1; i++) {

      if (ini.findSection(to_string(validPins[i]), buffer, len, IniFileState::funcFindSection)){ //found the section

        for (j = 0; j < entries.size()-1; j++) { //entries for each connected sensor

          if (ini.getValue(to_string(validPins[i]), entries[j], buffer, bufferLen)){

          } else {
            PRINT("Did not find the following field for sensor on pin ");
            PRINT(to_string(validPins[i]));
            PRINT(": ");
            PRINTLN(entries[j]);
          }
        }

      } else {
        PRINTLN("didn't find the section. Delete me.")
      }

    }
}




}
