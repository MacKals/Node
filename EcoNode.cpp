//  EcoNode.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoNode.hpp"


//Constructor
EcoNode::EcoNode() {
}

void EcoNode::init() {
	PRINT("Init node class.");

	this->setRTC();

	this->radio.init();
	this->sensors.init();
}

// can't change address live
// value between 1 and 63
int EcoNode::getAddress(){
   return 1*digitalRead(DIP0) + 2*digitalRead(DIP1) + 4*digitalRead(DIP2) + 8*digitalRead(DIP3) + 16*digitalRead(DIP4) + 32*digitalRead(DIP5);
}


// TIME

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void EcoNode::setRTC() {

	setSyncProvider(getTeensy3Time);

	if (timeStatus()!= timeSet) {
		PRINT("Unable to sync with the RTC");
	} else {
		PRINT("RTC has set the system time");
		PRINT(this->timeAsString());
	}
}


String digitsToString(int digits){
	// utility function for digital clock display: prints preceding colon and leading 0
	String s = ":";
	if (digits < 10) s += '0';
	s += String(digits);
	return s;
}

String EcoNode::timeAsString() {

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
