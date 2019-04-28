//  EcoNode.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoNode.hpp"


void EcoNode::init() {
	PRINTLN("Node class initializing.");

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

	EcoTime::setRTC();

	this->radio.init();
	this->sensors.init();
}


void EcoNode::loop() {

	this->radio.loop();

	// send message every 10 seconds
	if (EcoTime::timerDone()) {
		this->sendData();
		this->timer.startTimer(DATA_RECORD_INTERVAL);
	}

	return this->loop(); // infinite loop
}

void EcoNode::sendData() {
	PRINTLN("sending data");
	this->radio.send(sensors.getFullDataString());
}


// can't change address live
// value between 1 and 63
int EcoNode::getAddress(){
   return 1*digitalRead(DIP0) + 2*digitalRead(DIP1) + 4*digitalRead(DIP2) + 8*digitalRead(DIP3) + 16*digitalRead(DIP4) + 32*digitalRead(DIP5);
}
