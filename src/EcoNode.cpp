//  EcoNode.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoNode.hpp"

#include <Time.h>
#include <TimeAlarms.h>

void EcoNode::activateLED(bool on) {
	pinMode(LED, OUTPUT);
	digitalWrite(LED, on);
	ledStatus = on;
}

void EcoNode::blinkLED() {
	digitalWrite(LED, ledStatus);
	ledStatus = !ledStatus;
}

void EcoNode::init() {
	PRINTLN("Node class initializing.");
	activateLED();

	config_teensy35 = new SnoozeBlock(alarm);

	initBootCount();
	setRTC();
	sd.init();
	radio.init(sd);
	sensorMaster.init(sd);
	// gps.init();
	//setSensorParameters();

	recordConfiguraton();
	activateLED(false); // turn LED off
}

void EcoNode::initBootCount() {

	if (EEPROM.read(EEPROM_INIT_ADDRESS) == EEPROM_INIT_VAL) {
		// device has been booted with this code before

		uint8_t LSB = EEPROM.read(EEPROM_BOOTCOUNT_LSB_ADDRESS);
		uint8_t MSB = EEPROM.read(EEPROM_BOOTCOUNT_MSB_ADDRESS);

		// merge to bootCount and increment for current boot
		bootCount = LSB + (MSB << 8) + 1;

		// split into parts
		uint8_t newLSB = bootCount & 0xff;
		uint8_t newMSB = (bootCount >> 8);

		// PRINTLN("Previous bootCount:\t MSB=" + String(MSB) + ", LSB=" + String(LSB));
		// PRINTLN("New bootCount: \t\t MSB=" + String(newMSB) + ", LSB=" + String(newLSB));

		// write new boot-count to sd card
		if (newLSB != LSB) EEPROM.write(EEPROM_BOOTCOUNT_LSB_ADDRESS, newLSB);
		if (newMSB != MSB) EEPROM.write(EEPROM_BOOTCOUNT_MSB_ADDRESS, newMSB);

	} else {
		// device has not been booted with this code before
		EEPROM.write(EEPROM_INIT_ADDRESS, EEPROM_INIT_VAL); // register boot

		bootCount = 0;
		EEPROM.write(EEPROM_BOOTCOUNT_LSB_ADDRESS, 0);
		EEPROM.write(EEPROM_BOOTCOUNT_MSB_ADDRESS, 0);
	}

	PRINTLN("Boot count = " + String(bootCount));
}


void EcoNode::loop() {

	// if (gpsTimer.timerDone()) {
	// 	gps.printData();
	// 	gpsTimer.startTimer(TRANSMIT_INTERVAL);
	// }

	// read data to file
	if (dataTimer.timerDone()) {
		recordDataPacket();
		dataTimer.startTimer(RECORD_INTERVAL);
		PRINTLN("reading done");
	}

	// send data from file
	if (radioTimer.timerDone()) {
		sendDataPacket();
		radioTimer.startTimer(TRANSMIT_INTERVAL);
		PRINTLN("radio done");
	}

	// schedule sleep
	uint8_t sec = dataTimer.minSecondsLeft(radioTimer);
	uint8_t min = dataTimer.minMinutesLeft(radioTimer);
	uint8_t hour = dataTimer.minHoursLeft(radioTimer);

	PRINTLN("Delay for: " + String(hour) +":" + String(min) + ":" + String(sec));

    alarm.setRtcTimer(hour, min, sec); // hour, min, sec, wake again after this time
	delay(1000);

	int i = Snooze.sleep( *config_teensy35 );
	blinkLED();
}

// send data with radio if there is cached data to send
void EcoNode::sendDataPacket() {
	if (sd.cachedData() && radio.ready()) {
		String data = sd.popData();

		// try to send message and cache again if not successful
		radio.send(data);
		while (radio.transmitting()) {
			radio.loop();
		}
		if (!radio.transmitSuccessfull()) sd.cachData(data);
	}
}

// read information from sensors and cache information for sending
void EcoNode::recordDataPacket() {
	String data = String(bootCount) + "&";
	data += String(Teensy3Clock.get()); // s, time since 1.1.1970 (unix time)
	data += sensorMaster.getFullDataString();
	PRINTLN("Read data packet: " + data);
	sd.cachData(data);
}

// read configuration from sensors and cache information for sending
// C as first character indicates a config message
void EcoNode::recordConfiguraton() {
	String config = "C" + String(bootCount) + "&";
	config += String(Teensy3Clock.get());
	config += sensorMaster.getFullConfiguration();
	PRINTLN("Read config: " + config);
	sd.cachData(config);
}


// ---- RTC ----

String digitsToString(int digits){
	// utility function for digital clock display: prints preceding colon and leading 0
	String s = ":";
	if (digits < 10) s += '0';
	s += String(digits);
	return s;
}

// digital clock display of the time
String timeAsString() {
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
