//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoNode.hpp"


EcoNode node;

void setup() {
    Serial.begin(9600);
    delay(1000);

    node.init(); // init the EcoNode object

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

	delay(1000);

}

void loop() {
    Serial.println("loop");
	node.testFunc();
    //node.execute();

    delay(1000);
}
