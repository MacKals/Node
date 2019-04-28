//  SDISensor.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#include "SDISensor.h"


// Sensor methods

String SDISensor::readDataToString() {
    String allData = "";
    for (int i = 0; 0 < this->sensorAddresses.length(); i++) {
        char addr = this->sensorAddresses.charAt(i);
        allData += takeMeasurement(addr);
    }
    return allData;
}

bool SDISensor::sensorPresent() {
    String addr = this->sensorAddresses;
    return (addr.length() > 0) && !(addr.equals(" "));
}

// SDISensor spesific methods

void SDISensor::init() {

    // SDI-12 bus must be initialized with begin method
    this->sdiBus.begin();
    delay(30); // TODO: incrase? decrease?

    this->sensorAddresses = this->getFirstActiveAddress();
}

char SDISensor::getFirstActiveAddress() {
    for (byte addr = '0'; addr <= '9'; addr++) if (this->checkActive(addr)) {return (char) addr;}   // scan address space 0-9
    for (byte addr = 'a'; addr <= 'z'; addr++) if (this->checkActive(addr)) {return (char) addr;}   // scan address space a-z
    for (byte addr = 'A'; addr <= 'Z'; addr++) if (this->checkActive(addr)) {return (char) addr;}   // scan address space A-Z

    PRINTLN("no active address found");
    return ' ';
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
	this->sdiBus.read(); // consume address
	while(this->sdiBus.available()) {
		char c = this->sdiBus.read();
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
void SDISensor::printInfo(char addr){
	String command = "";
	command += (char) addr;
	command += "I!";
	this->sdiBus.sendCommand(command);
	// Serial.print(">>>");
	// Serial.println(command);
	delay(30);

	Serial.println(printBufferToString());
}


String SDISensor::takeMeasurement(char addr){
	String command = "";
	command += addr;
	command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
	this->sdiBus.sendCommand(command);
	delay(30);

	// wait for acknowlegement with format [address][ttt (3 char, seconds)][number of measurments available, 0-9]
	String sdiResponse = "";
	delay(30);
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
		if(this->sdiBus.available()) // sensor can interrupt us to let us know it is done early
		{
			this->sdiBus.clearBuffer();
			break;
		}
	}

	// Wait for anything else and clear it out
	delay(300);
	this->sdiBus.clearBuffer();

	// in this example we will only take the 'DO' measurement
	command = "";
	command += addr;
	command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
	this->sdiBus.sendCommand(command);
	while(!this->sdiBus.available()>1); // wait for acknowlegement
	delay(300); // let the data transfer

	String data = printBufferToString();
	this->sdiBus.clearBuffer();
	return data;
}

// this checks for activity at a particular address
// expects a char, '0'-'9', 'a'-'z', or 'A'-'Z'
bool SDISensor::checkActive(char addr){

	String myCommand = "";
	myCommand = "";
	myCommand += (char) addr;           // sends basic 'acknowledge' command [address][!]
	myCommand += "!";

	for(int j = 0; j < 3; j++) {     // goes through three rapid contact attempts
		this->sdiBus.sendCommand(myCommand);

		delay(30);
		if(this->sdiBus.available()) { // If we here anything, assume we have an active sensor
			//Serial.println(this->printBufferToString());
			this->sdiBus.clearBuffer();
			return true;
		}
	}
	this->sdiBus.clearBuffer();

	return false;
}

bool SDISensor::addressAttached(char addr) {
	return this->sensorAddresses.indexOf(addr) == -1 ? false : true;
}

bool SDISensor::changeAddress(char from, char to) {
	PRINTLN("Address " + String(from) + " changed to " + String(to));

	String command = "";
	command += (char) from;
	command += "A";
	command += (char) to;
	command += "!";
	this->sdiBus.sendCommand(command);
	this->sensorAddresses.replace(from, to);
	delay(300);

	this->sdiBus.clearBuffer();
}
