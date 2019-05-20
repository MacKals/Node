//  SDISensor.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#include "SDISensor.h"


// Sensor methods

String SDISensor::readDataToString() {
    if (canHandleContinuousMeasurement) return takeContinuousMeasurement();
    return takeMeasurement();
}

// SDISensor specific methods

void SDISensor::init() {
    // SDI-12 bus must be initialized with begin method
    sdiBus.begin();
    delay(30);
    sensorAddress = getFirstActiveAddress();

    // now to see if continuous measurements are supported
    String command = String(sensorAddress) + "R7!"; //currently just suppports ATMOS 41 weather station
    this->sdiBus.sendCommand(command);
    //PRINTLN("sending: " + command);
    delay(100);

    String response = printBufferToString();
    if (response.compareTo("") == 0){ // returns nothing
        canHandleContinuousMeasurement = false;
    }
    //PRINTLN("R! command response: " + response);
}

void SDISensor::end() {
    sdiBus.end();
}

char SDISensor::getFirstActiveAddress() {
    for (byte addr = '0'; addr <= '9'; addr++) if (this->checkActive(addr)) {return (char) addr;}   // scan address space 0-9
    for (byte addr = 'a'; addr <= 'z'; addr++) if (this->checkActive(addr)) {return (char) addr;}   // scan address space a-z
    for (byte addr = 'A'; addr <= 'Z'; addr++) if (this->checkActive(addr)) {return (char) addr;}   // scan address space A-Z

    return ' '; // no active address found
}

String SDISensor::getAllActiveAddresses() {
    PRINTLN("Attaching all SDI-12 nodes.");

    String addresses = "";

    // serach through all valid addresses
    for (byte addr = '0'; addr <= '9'; addr++) if (this->checkActive(addr)) {addresses += (char) addr;}   // scan address space 0-9
    for (byte addr = 'a'; addr <= 'z'; addr++) if (this->checkActive(addr)) {addresses += (char) addr;}   // scan address space a-z
    for (byte addr = 'A'; addr <= 'Z'; addr++) if (this->checkActive(addr)) {addresses += (char) addr;}   // scan address space A-Z

    PRINTLN("Found " + String(addresses.length()) + " at " + addresses);
    return addresses;
}


String SDISensor::printBufferToString() {
	String buffer = "";
	sdiBus.read(); // consume address
    sdiBus.read(); // consume comma
	while(sdiBus.available()) {
		char c = sdiBus.read();
		if(c == '+') {
			if (buffer.length() != 0) buffer += ',';
		} else if (c == '-') {
			buffer += ",-";
		} else if ((c != '\n') && (c != '\r') && (c != '\t')) {
			buffer += c;
		}
		delay(50);
	}

	return buffer;
}

// gets identification information from a sensor, and prints it to the serial port
// expects a character between '0'-'9', 'a'-'z', or 'A'-'Z'.
String SDISensor::printInfoToString(){

    this->sdiBus.setActive();
    sdiBus.clearBuffer();

	String command = String(sensorAddress) + "I!";
	// command += (char) addr;
	// command += "I!";
	this->sdiBus.sendCommand(command);
	delay(100);

    String info = printBufferToString();
    sdiBus.clearBuffer();
    this->sdiBus.forceHold();
    return info;
}

void SDISensor::printInfo() {
    PRINTLN("Pin: " + String(pin) + ", addr: " + String(sensorAddress) + ", desc: " + printInfoToString());
}

String SDISensor::takeMeasurement(){ //using M! and D! commands

    this->sdiBus.setActive();

	String command = "";
	command += String(sensorAddress);
	command += "M!"; // SDI-12 measurement command format  [address]['M'][!]

    this->sdiBus.sendCommand(command);
	delay(60);

	// wait for acknowlegement with format [address][ttt (3 char, seconds)][number of measurments available, 0-9]
	String sdiResponse = "";
	while (this->sdiBus.available()) // build response string
	{
		char c = this->sdiBus.read();
		if ((c != '\n') && (c != '\r'))
		{
			sdiResponse += c;
			delay(5);
		}
	}

	this->sdiBus.clearBuffer();

	// find out how long we have to wait (in seconds).
	uint16_t wait = 0;
	wait = sdiResponse.substring(1,4).toInt();

	// Set up the number of results to expect
	// int numMeasurements =  sdiResponse.substring(4,5).toInt();

	unsigned long timerStart = millis();

	while((millis() - timerStart) < (1000 * wait)) {
		if(this->sdiBus.available()) { // sensor can interrupt us to let us know it is done early
			this->sdiBus.clearBuffer();
			break;
		}
	}

	// Wait for anything else and clear it out
	delay(300);
	this->sdiBus.clearBuffer();

	// in this example we will only take the 'DO' measurement
	command = "";
	command += String(sensorAddress);
	command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
	this->sdiBus.sendCommand(command);

    timerStart = millis();
	while (!(this->sdiBus.available()>1)) {
        // wait for acknowlegement

        // timeout
        if ((millis() - timerStart) > (10000 * wait)) {
            return "000";
        }
    }

    delay(300); // let the data transfer

	String data = printBufferToString();

	this->sdiBus.clearBuffer();
    this->sdiBus.forceHold();

	return data;
}

String SDISensor::takeContinuousMeasurement(){

    this->sdiBus.setActive();

	String command = String(sensorAddress) + "R7!";

    this->sdiBus.sendCommand(command);
	delay(60);

	String sdiResponse = printBufferToString();

	this->sdiBus.clearBuffer();
    this->sdiBus.forceHold();

	return sdiResponse;
}

// this checks for activity at a particular address
// expects a char, '0'-'9', 'a'-'z', or 'A'-'Z'
bool SDISensor::checkActive(char addr){

    this->sdiBus.setActive();

	String myCommand = "";
	myCommand += (char) addr;        // sends basic 'acknowledge' command [address][!]
	myCommand += "!";

	for(int j = 0; j < 3; j++) {     // goes through three rapid contact attempts
		this->sdiBus.sendCommand(myCommand);

		delay(30);
		if(this->sdiBus.available()) { // If we here anything, assume we have an active sensor
			// PRINTLN(this->printBufferToString());
			this->sdiBus.clearBuffer();
            this->sdiBus.forceHold();

            return true;
		}
	}
	this->sdiBus.clearBuffer();
    this->sdiBus.forceHold();

	return false;
}

bool SDISensor::addressAttached(char addr) {
	return this->sensorAddress == addr ? false : true;
}

void SDISensor::changeAddress(char from, char to) {
	PRINTLN("Address " + String(from) + " changed to " + String(to));

    this->sdiBus.setActive();
	String command = "";
	command += (char) from;
	command += "A";
	command += (char) to;
	command += "!";
	this->sdiBus.sendCommand(command);
	//this->sensorAddress.replace(from, to);
	delay(300);

	this->sdiBus.clearBuffer();

    this->sdiBus.forceHold();
}
