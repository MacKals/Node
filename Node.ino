//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoNode.hpp"
#include "Configuration.hpp"
EcoNode node;

void setup() {

    #ifdef DEBUG
        Serial.begin(115200);
        // while (!Serial) {}
    #endif

    delay(200);

    node.init();
}

void loop() {
    node.loop();
}
