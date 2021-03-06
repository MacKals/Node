/*
   Example H:  SDI-12 slave implementation
   Example sketch demonstrating how to implement an arduino as a slave on an SDI-12 bus. This may
   be used, for example, as a middleman between an I2C sensor and an SDI-12 datalogger.

   Note that an SDI-12 slave must respond to M! or C! with the number of values it will report and
   the max time until these values will be available.  This example uses 9 values available in
   21 s, but references to these numbers and the output array size and datatype should be changed
   for your specific application.

   Sketch requires the SDI-12 library from SWRC, modified to add public void forceListen() and
   public void sendResponse().
   https://github.com/dwasielewski/Arduino-SDI-12

   D. Wasielewski, 2016
   Builds upon work started by:
   https://github.com/jrzondagh/AgriApps-SDI-12-Arduino-Sensor
   https://github.com/Jorge-Mendes/Agro-Shield/tree/master/SDI-12ArduinoSensor

   Suggested improvements:
    - Get away from memory-hungry arduino String objects in favor of char buffers
*/


#include <SDI12.h>

#define DATA_PIN 3         // The pin of the SDI-12 data bus

#define BLUE A0        // blue led pin
#define GREEN A1       // green led pin
#define RED A2         // red led pin
#define IR A3          // ir led pin

#define DATA_COUNT 4  // number of data-points that will be transmitted over SDI-12

#define V_REF 4.096   // V, analog voltage refference
#define ADC_MAX 1024  // max reading of adc, 2^10 

#define POWER_PIN -1       // The sensor power pin (or -1 if not switching power)

char sensorAddress = '2';
int state = 0;

#define WAIT 0
#define INITIATE_CONCURRENT 1
#define INITIATE_MEASUREMENT 2

SDI12 slaveSDI12(DATA_PIN);


float LEDRead(uint8_t pin) {
  return ((float) analogRead(pin)) / ADC_MAX;
}

void pollSensor(float* measurementValues) {
  measurementValues[0] = LEDRead(BLUE);
  measurementValues[1] = LEDRead(GREEN);
  measurementValues[2] = LEDRead(RED);
  measurementValues[3] = LEDRead(IR);
}


void parseSdi12Cmd(String command, String* dValues) {
  /* Ingests a command from an SDI-12 master, sends the applicable response, and
     (when applicable) sets a flag to initiate a measurement
  */

  // First char of command is always either (a) the address of the device being
  // probed OR (b) a '?' for address query.
  // Do nothing if this command is addressed to a different device
  if (command.charAt(0) != sensorAddress && command.charAt(0) != '?') {
    //Serial.println(command);
    return;
  }
  //Serial.println("M" + command);

  // If execution reaches this point, the slave should respond with something in
  // the form:   <address><responseStr><Carriage Return><Line Feed>
  // The following if-switch-case block determines what to put into <responseStr>,
  // and the full response will be constructed afterward. For '?!' (address query)
  // or 'a!' (acknowledge active) commands, responseStr is blank so section is skipped
  String responseStr = "";
  if (command.length() > 1) {
    switch (command.charAt(1)) {
      case 'I':
        // Identify command
        // Slave should respond with ID message: 2-char SDI-12 version + 8-char
        // company name + 6-char sensor model + 3-char sensor version + 0-13 char S/N
        responseStr = "13UBC_ECOH0000011.0001"; // Substitute proper ID String here
        break;
      case 'C':
        // Initiate concurrent measurement command
        // Slave should immediately respond with: "tttnn":
        //    3-digit (seconds until measurement is available) +
        //    2-digit (number of values that will be available)
        // Slave should also start a measurment and relinquish control of the data line
        responseStr = "00024";  // 4 values ready in 0.2 sec; Substitue sensor-specific values here
        // It is not preferred for the actual measurement to occur in this subfunction,
        // because doing to would hold the main program hostage until the measurement
        // is complete.  Instead, we'll just set a flag and handle the measurement elsewhere.
        state = INITIATE_CONCURRENT;
        break;
      // NOTE: "aC1...9!" commands may be added by duplicating this case and adding
      //       additional states to the state flag
      case 'M':
        // Initiate measurement command
        // Slave should immediately respond with: "tttnn":
        //    3-digit (seconds until measurement is available) +
        //    1-digit (number of values that will be available)
        // Slave should also start a measurment but may keep control of the data line until
        // advertised time elapsed OR measurement is complete and service request sent
        responseStr = "0014";  // 4 values ready in 1 sec; Substitue sensor-specific values here
        // It is not preferred for the actual measurement to occur in this subfunction,
        // because doing to would hold the main program hostage until the measurement
        // is complete.  Instead, we'll just set a flag and handle the measurement elsewhere.
        // It is preferred though not required that the slave send a service request upon
        // completion of the measurement.  This should be handled in the main loop().
        state = INITIATE_MEASUREMENT;
        break;
      // NOTE: "aM1...9!" commands may be added by duplicating this case and adding
      //       additional states to the state flag

      case 'D':
        // Send data command
        // Slave should respond with a String of values
        // Values to be returned must be split into Strings of 35 characters or fewer
        // (75 or fewer for concurrent).  The number following "D" in the SDI-12 command
        // specifies which String to send
        responseStr = dValues[(int)command.charAt(2) - 48];
        break;
      case 'A':
        // Change address command
        // Slave should respond with blank message (just the [new] address + <CR> + <LF>)
        sensorAddress = command.charAt(2);
        break;
      default:
        // Mostly for debugging; send back UNKN if unexpected command received
        responseStr = "UNKN";
        break;
    }
  }

  // Issue the response speficied in the switch-case structure above.
  slaveSDI12.sendResponse(String(sensorAddress) + responseStr + "\r\n");
}


void formatOutputSDI(float* measurementValues, String* dValues, unsigned int maxChar) {
  /* Ingests an array of floats and produces Strings in SDI-12 output format */

  dValues[0] = "";
  int j = 0;

  // upper limit on i should be number of elements in measurementValues
  for (int i = 0; i < DATA_COUNT; i++) {
    // Read float value "i" as a String with 6 deceimal digits
    // (NOTE: SDI-12 specifies max of 7 digits per value; we can only use 6
    //  decimal place precision if integer part is one digit)
    String valStr = String(measurementValues[i], 3);

    if (valStr.charAt(0) != '-') valStr = '+' + valStr; // Explictly add implied + sign if non-negative

    // Append dValues[j] if it will not exceed 35 (aM!) or 75 (aC!) characters
    if (dValues[j].length() + valStr.length() < maxChar) {
      dValues[j] += valStr;
    }
    // Start a new dValues "line" if appending would exceed 35/75 characters
    else {
      dValues[++j] = valStr;
    }
  }

  // Fill rest of dValues with blank strings
  while (j < 9) {
    dValues[++j] = "";
  }
}


void setup() {
  Serial.begin(9600);

  Serial.println(OSCCAL, BIN);
  OSCCAL += 4;
  Serial.println(OSCCAL, BIN);

  slaveSDI12.begin();
  delay(500);
  slaveSDI12.forceListen();  // sets SDIPIN as input to prepare for incoming message
}

static float measurementValues[DATA_COUNT]; // floats to hold simulated sensor data
static String dValues[DATA_COUNT + 1]; // 5 String objects to hold the responses to aD0!-aD9! commands
static String commandReceived = "";  // String object to hold the incoming command

int counter = 0;


void printBinary(byte inByte)
{
  for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(inByte, b));
  }
}


void loop() {
  // If a byte is available, an SDI message is queued up. Read in the entire message
  // before proceding.  It may be more robust to add a single character per loop()
  // iteration to a static char buffer; however, the SDI-12 spec requires a precise
  // response time, and this method is invariant to the remaining loop() contents.
  int avail = slaveSDI12.available();
  if (avail < 0) {
    slaveSDI12.clearBuffer();  // Buffer is full; clear
  }
  else if (avail > 0) {
    for (int a = 0; a < avail; a++) {

      //      if (counter == 20) {
      //       // OSCCAL += 1;
      //        Serial.println(OSCCAL, BIN);
      //        counter = 0;
      //      }
      //      counter++;

      char charReceived = slaveSDI12.read();
      // Character '!' indicates the end of an SDI-12 command; if the current
      // character is '!', stop listening and respond to the command
      if (charReceived == '?') Serial.print(charReceived);
      else printBinary((byte) charReceived);
      Serial.println("");

      if (charReceived == '!') {
        // Command string is completed; do something with it
        parseSdi12Cmd(commandReceived, dValues);
        // Clear command string to reset for next command
        commandReceived = "";
        // '!' should be the last available character anyway, but exit the "for" loop
        // just in case there are any stray characters
        slaveSDI12.clearBuffer();
        break;
      }
      // If the current character is anything but '!', it is part of the command
      // string.  Append the commandReceived String object.
      else {
        // Append command string with new character
        commandReceived += String(charReceived);
      }
    }
  }


  // For aM! and aC! commands, parseSdi12Cmd will modify "state" to indicate that
  // a measurement should be taken
  switch (state) {
    case WAIT:
      break;
    case INITIATE_CONCURRENT:
      //Serial.println("INITIATE_CONCURRENT");
      // Get sensor data
      pollSensor(measurementValues);
      // Populate the "dValues" String array with the values in SDI-12 format
      formatOutputSDI(measurementValues, dValues, 75);
      state = WAIT;
      break;
    case INITIATE_MEASUREMENT:
      //Serial.println("INITIATE_MEASUREMENT");
      // Get sensor data
      pollSensor(measurementValues);
      // Populate the "dValues" String array with the values in SDI-12 format
      formatOutputSDI(measurementValues, dValues, 35);
      // For aM!, Send "service request" (<address><CR><LF>) when data is ready
      slaveSDI12.sendResponse(String(sensorAddress) + "\r\n");
      state = WAIT;
      break;
  }
}
