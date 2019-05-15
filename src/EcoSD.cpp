
#include "EcoSD.hpp"
using namespace std;

bool EcoSD::init() {

    if (SD.begin(chipSelect)) {
        initialized = true;
        PRINTLN("SD card initialized.");

        // file caching details
        if (!SD.exists(cachDirectory.c_str())) {
            SD.mkdir(cachDirectory.c_str());
        }

        // set cachNumber to current file head
        cachNumber = 0;
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
    cachNumber--;
    PRINTLN("Failed to cach data");
    return false;
}

String EcoSD::popData() {
    // return if SD card not initialized
    if (!initialized || !cachedData()) return "";

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

// sends all sensors, each with format of {"Pin", "Sensor Type", "SN"}
vector<vector<String>> EcoSD::getSensorsFromConfig(const vector<uint8_t> validPins) {

    vector<vector<String>> retvec;
    vector<String> cur;
    const size_t bufferLen = 100;
    char buffer[bufferLen];

    if (!ini.open()) {
      PRINTLN("ini file " + String(ini.getFilename()) + " not valid.");
      return retvec;
    }

    PRINTLN("ini file exists, reading values...");

    // expected fields for each sensor
    // const string entryType = "type";
    // const string entrySN = "SN";

    vector<String> fields = {"type", "SN"};

    for (auto& pin : validPins) {

        //PRINTLN("LF " + String(pin) + " " + String(entryType));

        if (ini.getValue(String(pin).c_str(), fields[0].c_str(), buffer, bufferLen)) { //found the section (by checking entryType field)

            cur = {String(pin)};
            //cur.push_back(String(pin));

            for (auto &field : fields){

                if (ini.getValue(String(pin).c_str(), field.c_str(), buffer, bufferLen)){
                    cur.push_back(String(buffer));
                } else {
                    PRINTLN("Did not find " + String(field) + " for sensor on pin " + String(pin));
                    break;
                }
            }

            PRINTLN("from config: pin: " + String(pin) + ", type: " + cur[1] + ", SN: " + cur[2]);
            retvec.push_back(cur);
        } else {
            PRINTLN("Nothing connected on pin: " + String(pin));
        }
    }
    ini.close();
    return retvec;
}
