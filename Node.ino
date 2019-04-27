//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoNode.hpp"


EcoNode node;

void setup() {
    Serial.begin(115200);
	while (!Serial);  // Wait for Arduino Serial Monitor to open

    delay(100);

	PRINTLN("Started.");
	delay(500);

    node.init(); // init the EcoNode object

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

	delay(1000);

	PRINTLN("Init complete, entering loop.");
	node.sendData();

}

void loop() {
    // PRINTLN("loop");

	node.loop();

    //node.execute();

    // delay(1000);
}
