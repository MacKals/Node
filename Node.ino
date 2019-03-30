//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoNode.hpp"

//#include <SDI12.h>

EcoNode node;

void setup() {
    Serial.begin(9600);
    delay(1000);

    node.init(); // init the EcoNode object

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    node.printRegisters();
}

void loop() {
    Serial.println("loop");

    node.execute();

    delay(1000);
}
