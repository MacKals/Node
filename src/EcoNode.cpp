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

	// gps.init();

	sd.init();
	setLoRaParameters();
	radio.init();

	// sensorMaster.init();
	setSensorParameters();
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
	String data = sd.getDataFromFile(LORAWAN_CONFIG_FILE_NAME);

	if (data.length() == 0) return;

	uint16_t i1 = data.indexOf('\n');
	uint16_t i2 = data.indexOf('\n', i1+1);

	String appeui = cleanupString(data.substring(0, i1));
	String deveui = cleanupString(data.substring(i1, i2));
	String appkey = cleanupString(data.substring(i2)); // from index to end of string

	radio.setLoRaParameters(appeui, deveui, appkey);
}

// must be called after sd init
// gets all sensor types, pins, ranges and types from file and configures setup
void EcoNode::setSensorParameters() {
	PRINTLN("Setting sensor parameters");
	String data = sd.getDataFromFile(SENSORS_CONFIG_FILE_NAME);

	int i = data.indexOf('\n');

	while (i != -1) {
		sensorMaster.initSensorFromString(data.substring(0,i)); // process first line in file
		data = data.substring(i+1); // remove line that was just processed
		i = data.indexOf('\n');     // get next line end
	}

	sensorMaster.initSensorFromString(data); // execute final command at end of file
	PRINTLN("done initializing sensors");
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

		// gps.printData();

		blinkLED();

		String data = this->sensorMaster.getFullDataString();
		// sendData(data);

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
