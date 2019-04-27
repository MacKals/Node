//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoNode.hpp"

EcoNode node;

void setup() {

    Serial.begin(115200);
    delay(100);

    node.init();
}

void loop() {
    node.loop();
}
