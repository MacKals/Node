

#include <Arduino.h>
#include "EcoSD.hpp"

EcoSD sd;

void setup() {
    Serial.begin(9600);

    delay(1000);

    sd.init();
}

void loop() {
    // Serial.println("looping");
    if (Serial.available()) {
        char c = Serial.read();
        Serial.print(String(c) + " ");
        if (c == '1') sd.cachData(F("appleBees"));
        if (c == '2') {
            String dat = sd.popData();
            Serial.println(sd.cachFileName() + "\t popped " + dat);
        }
        if (c == '3') Serial.println(sd.cachFileName());
    }
    delay(10);
}
