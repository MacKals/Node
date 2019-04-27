//  EcoSensor.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensor.hpp"


String EcoSensor::printBufferToString() {
	String buffer = "";
	this->sdi12.read(); // consume address
	while(this->sdi12.available()) {
		char c = this->sdi12.read();
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
void EcoSensor::printInfo(char i){
	String command = "";
	command += (char) i;
	command += "I!";
	this->sdi12.sendCommand(command);
	// Serial.print(">>>");
	// Serial.println(command);
	delay(30);

	Serial.println(printBufferToString());
}


String EcoSensor::takeMeasurement(char i){
	String command = "";
	command += i;
	command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
	this->sdi12.sendCommand(command);
	delay(30);

	// wait for acknowlegement with format [address][ttt (3 char, seconds)][number of measurments available, 0-9]
	String sdiResponse = "";
	delay(30);
	while (this->sdi12.available()) // build response string
	{
		char c = this->sdi12.read();
		if ((c != '\n') && (c != '\r'))
		{
			sdiResponse += c;
			delay(5);
		}
	}

	this->sdi12.clearBuffer();

	// find out how long we have to wait (in seconds).
	uint16_t wait = 0;
	wait = sdiResponse.substring(1,4).toInt();

	// Set up the number of results to expect
	// int numMeasurements =  sdiResponse.substring(4,5).toInt();

	unsigned long timerStart = millis();

	while((millis() - timerStart) < (1000 * wait)) {
		if(this->sdi12.available()) // sensor can interrupt us to let us know it is done early
		{
			this->sdi12.clearBuffer();
			break;
		}
	}

	// Wait for anything else and clear it out
	delay(300);
	this->sdi12.clearBuffer();

	// in this example we will only take the 'DO' measurement
	command = "";
	command += i;
	command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
	this->sdi12.sendCommand(command);
	while(!this->sdi12.available()>1); // wait for acknowlegement
	delay(300); // let the data transfer

	String data = printBufferToString();
	this->sdi12.clearBuffer();
	return data;
}

// this checks for activity at a particular address
// expects a char, '0'-'9', 'a'-'z', or 'A'-'Z'
bool EcoSensor::checkActive(char i){

	String myCommand = "";
	myCommand = "";
	myCommand += (char) i;           // sends basic 'acknowledge' command [address][!]
	myCommand += "!";

	for(int j = 0; j < 3; j++) {     // goes through three rapid contact attempts
		this->sdi12.sendCommand(myCommand);

		delay(30);
		if(this->sdi12.available()) { // If we here anything, assume we have an active sensor
			//Serial.println(this->printBufferToString());
			this->sdi12.clearBuffer();
			return true;
		}
	}
	this->sdi12.clearBuffer();

	return false;
}

bool EcoSensor::addressAttached(char i) {
	return this->sdi12_addr.indexOf(i) == -1 ? false : true;
}

bool EcoSensor::changeAddress(char from, char to) {
	PRINTLN("Address " + String(from) + " changed to " + String(to));

	String command = "";
	command += (char) from;
	command += "A";
	command += (char) to;
	command += "!";
	this->sdi12.sendCommand(command);
	this->sdi12_addr.replace(from, to);
	delay(300);

	this->sdi12.clearBuffer();
}
