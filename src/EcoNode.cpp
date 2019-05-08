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
	digitalWrite(LED, ledStatus);
	ledStatus = !ledStatus;
}

void EcoNode::init() {
	PRINTLN("Node class initializing.");

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	Alarm.timerRepeat(2, blinkLED);

	setRTC();

	sd.init();
	setLoRaParameters();
	PRINTLN("set the parameters");
	radio.init();
	sensors.init();

}

// TODO: Use?
String EcoNode::cleanupString(String s) {
	s = s.replace(" ", "");
	s = s.replace("\n", "");
	s = s.replace("\t", "");
	return s;
}

// must be called after sd init and before radio init (?)
// get parameters for LoRaWAN transmission from SD card
void EcoNode::setLoRaParameters() {
	String data = sd.getLoRaConfigData();

	if (data.length() == 0) {
		PRINTLN("Config file not found");
		return;
	}

	uint16_t i1 = data.indexOf('\n');
	uint16_t i2 = data.indexOf('\n', i1+1);
	// PRINT("data in file: ");
	// PRINTLN(data);
	// PRINTLN(i1);
	// PRINTLN(i2);

	String appeui = cleanupString(data.substring(0, i1));
	String deveui = cleanupString(data.substring(i1, i2));
	String appkey = cleanupString(data.substring(i2)); // from index to end of string

	// PRINTLN(appeui);
	// PRINTLN(deveui);
	// PRINTLN(appkey);
	radio.setLoRaParameters(appeui, deveui, appkey);
}

void EcoNode::loop() {

	radio.loop();

	// if (radio.ready() && sd.cachedData()) {
	// 	String data = sd.popData();
	// 	PRINTLN("Popped data \t" + data);
	// 	sendData(data);
	// }

	// read data and send at given interval
	if (timer.timerDone()) {

		blinkLED();

		String data = this->sensors.getFullDataString();
		sendData(data);

		// start new timer for next data-collection
		timer.startTimer(DATA_RECORD_INTERVAL);
	}
}

void EcoNode::sendData(String data) {
	PRINTLN("sending data \t" + data);
	if (!this->radio.send(data)) {
		sd.cachData(data);
	}
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
