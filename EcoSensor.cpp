//  EcoSensor.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoSensor.hpp"


// converts allowable address characters '0'-'9', 'a'-'z', 'A'-'Z',
// to a decimal number between 0 and 61 (inclusive) to cover the 62 possible addresses
byte EcoSensor::charToDec(char i){
	if((i >= '0') && (i <= '9')) return i - '0';
	if((i >= 'a') && (i <= 'z')) return i - 'a' + 10;
	if((i >= 'A') && (i <= 'Z')) return i - 'A' + 37;
	else return i;
}

// maps a decimal number between 0 and 61 (inclusive) to
// allowable address characters '0'-'9', 'a'-'z', 'A'-'Z',
char EcoSensor::decToChar(byte i){
	if((i >= 0) && (i <= 9)) return i + '0';
	if((i >= 10) && (i <= 36)) return i + 'a' - 10;
	if((i >= 37) && (i <= 62)) return i + 'A' - 37;
	else return i;
}

// void EcoSensor::printBufferToScreen(){
// 	String buffer = "";
// 	EcoSensor->sdi12.read(); // consume address
// 	while(EcoSensor->sdi12.available()) {
// 		char c = EcoSensor->sdi12.read();
// 		if(c == '+') {
// 			if (buffer.length() != 0) {
// 				buffer += ',';
// 			}
// 		} else if (c == '-') {
// 			buffer += ",-";
// 		} else if ((c != '\n') && (c != '\r') && (c != '\t')) {
// 			buffer += c;
// 		}
// 		delay(50);
// 	}
//
// 	Serial.print(buffer);
// }
//
// // gets identification information from a sensor, and prints it to the serial port
// // expects a character between '0'-'9', 'a'-'z', or 'A'-'Z'.
// void EcoSensor::printInfo(char i){
// 	String command = "";
// 	command += (char) i;
// 	command += "I!";
// 	EcoSensor->sdi12.sendCommand(command);
// 	// Serial.print(">>>");
// 	// Serial.println(command);
// 	delay(30);
//
// 	printBufferToScreen();
// }
//
//
// void EcoSensor::takeMeasurement(char i){
// 	String command = "";
// 	command += i;
// 	command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
// 	EcoSensor->sdi12.sendCommand(command);
// 	delay(30);
//
// 	// wait for acknowlegement with format [address][ttt (3 char, seconds)][number of measurments available, 0-9]
// 	String sdiResponse = "";
// 	delay(30);
// 	while (EcoSensor->sdi12.available()) // build response string
// 	{
// 		char c = EcoSensor->sdi12.read();
// 		if ((c != '\n') && (c != '\r'))
// 		{
// 			sdiResponse += c;
// 			delay(5);
// 		}
// 	}
//
// 	EcoSensor->sdi12.clearBuffer();
//
// 	// find out how long we have to wait (in seconds).
// 	uint16_t wait = 0;
// 	wait = sdiResponse.substring(1,4).toInt();
//
// 	// Set up the number of results to expect
// 	// int numMeasurements =  sdiResponse.substring(4,5).toInt();
//
// 	unsigned long timerStart = millis();
//
// 	while((millis() - timerStart) < (1000 * wait)) {
// 		if(EcoSensor->sdi12.available()) // sensor can interrupt us to let us know it is done early
// 		{
// 			EcoSensor->sdi12.clearBuffer();
// 			break;
// 		}
// 	}
//
// 	// Wait for anything else and clear it out
// 	delay(300);
// 	EcoSensor->sdi12.clearBuffer();
//
// 	// in this example we will only take the 'DO' measurement
// 	command = "";
// 	command += i;
// 	command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
// 	EcoSensor->sdi12.sendCommand(command);
// 	while(!EcoSensor->sdi12.available()>1); // wait for acknowlegement
// 	delay(300); // let the data transfer
// 	printBufferToScreen();
// 	EcoSensor->sdi12.clearBuffer();
// }
//
// // this checks for activity at a particular address
// // expects a char, '0'-'9', 'a'-'z', or 'A'-'Z'
// bool EcoSensor::checkActive(char i){
//
// 	String myCommand = "";
// 	myCommand = "";
// 	myCommand += (char) i;           // sends basic 'acknowledge' command [address][!]
// 	myCommand += "!";
//
// 	for(int j = 0; j < 3; j++) {     // goes through three rapid contact attempts
// 		EcoSensor->sdi12.sendCommand(myCommand);
// 		delay(30);
// 		if(EcoSensor->sdi12.available()) { // If we here anything, assume we have an active sensor
// 			printBufferToScreen();
// 			EcoSensor->sdi12.clearBuffer();
// 			return true;
// 		}
// 	}
// 	EcoSensor->sdi12.clearBuffer();
// 	return false;
// }
//
//
// // this quickly checks if the address has already been taken by an active sensor
// bool EcoSensor::isTaken(byte i){
// 	i = charToDec(i); // e.g. convert '0' to 0, 'a' to 10, 'Z' to 61.
// 	byte j = i / 8; // byte #
// 	byte k = i % 8; // bit #
// 	return addressRegister[j] & (1<<k); // return bit status
// }
//
// // this sets the bit in the proper location within the addressRegister
// // to record that the sensor is active and the address is taken.
// bool EcoSensor::setTaken(byte i){
// 	bool initStatus = EcoSensor::isTaken(i);
// 	i = charToDec(i); // e.g. convert '0' to 0, 'a' to 10, 'Z' to 61.
// 	byte j = i / 8; // byte #
// 	byte k = i % 8; // bit #
// 	addressRegister[j] |= (1 << k);
// 	return !initStatus; // return false if already taken
// }
//
// // this unsets the bit in the proper location within the addressRegister
// // to record that the sensor is active and the address is taken.
// bool EcoSensor::setVacant(byte i){
// 	bool initStatus = isTaken(i);
// 	i = charToDec(i); // e.g. convert '0' to 0, 'a' to 10, 'Z' to 61.
// 	byte j = i / 8; // byte #
// 	byte k = i % 8; // bit #
// 	addressRegister[j] &= ~(1 << k);
// 	return initStatus; // return false if already vacant
// }
