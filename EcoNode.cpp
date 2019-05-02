//  EcoNode.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoNode.hpp"

#include <Time.h>
#include <TimeAlarms.h>

bool ledStatus = false;

void blinkLED() {
	PRINTLN("LED flip");
	digitalWrite(LED, ledStatus);
	ledStatus = !ledStatus;
}

void EcoNode::init() {
	PRINTLN("Node class initializing.");

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	Alarm.timerRepeat(2, blinkLED);

	this->setRTC();

	this->radio.init();
	this->sensors.init();
	this->sd.init();
}


void EcoNode::loop() {

	this->radio.loop();

	// if (radio.ready() && sd.cachedData()) {
	// 	radio.send(sd.popData());
	// }

	// read data and send at given interval
	if (this->timer.timerDone()) {
		blinkLED();
		this->sendData();
		this->timer.startTimer(DATA_RECORD_INTERVAL);
	}

	return this->loop(); // infinite loop
}

void EcoNode::sendData() {
	String data = this->sensors.getFullDataString();
	PRINTLN("sending data \t" + data);
	if (!this->radio.send(data)) {
		sd.cachData(data);
	}
}



// can't change address live
// value between 1 and 63
int EcoNode::getAddress(){
   return 1*digitalRead(DIP0) + 2*digitalRead(DIP1) + 4*digitalRead(DIP2) + 8*digitalRead(DIP3) + 16*digitalRead(DIP4) + 32*digitalRead(DIP5);
}




// ---- RTC ----

String digitsToString(int digits){
	// utility function for digital clock display: prints preceding colon and leading 0
	String s = ":";
	if (digits < 10) s += '0';
	s += String(digits);
	return s;
}

String timeAsString() {

	// digital clock display of the time
	String s = String(hour());
	s += digitsToString(minute());
	s += digitsToString(second());
	s += " ";
	s += String(day()) + " ";
	s += String(month()) + " ";
	s += String(year());
	return s;
}

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void EcoNode::setRTC() {
	setSyncProvider(getTeensy3Time);

	if (timeStatus()!= timeSet) {
		PRINTLN("Unable to sync with the RTC");
	} else {
		PRINTLN("RTC has set the system time");
		PRINTLN(timeAsString());
	}
}
