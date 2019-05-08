# Codebase for UBC Ecohydrology Sensor Nodes

The code in this repository is made to provide a flexible solution for connecting sensors to a [LoRa based sensor node](https://github.com/UBCecohydro/AgWaterMon_CAD). The code is developed around the Teensy 3.5 microcontroller development board.

to configure the system, you need to place two configuration files on a micro SD card that you insert into the Teensy. This SD card will also function as buffering for when the node cannot establish a connection with a LoRaWAN station.

## Supported Sensor Communication Protocols
- SDI-12 (max 12)
- Analog single ended (max 12)
- Flow sensors (max 12)
- PWM sensors (max 12)
- Analog differential (max 2, lacks software support)
- UART (2 headers, lacks software support)

All single wire interfaces can be connected and used with any of the three pin terminal blocks. A config file can be used to set limits for analog sensors etc.

## An overview of the codebase
- Launch the Node.ino file in the Arduino IDE to compile the library.
- The class EcoNode is launched as the top level system manager, performing scheduling and coordinating the functionality of the system. Functionality is encapsulated in subclasses in the following manner:
    - `EcoRadio` interfaces with the LoRa module and LoRaWAN communication protocol.
    - `EcoGPS` interfaces with the U-Blox GPS module.
    - `EcoSD` interfaces with the file system on the SD card. This includes buffering, storage and retrieval of dropped packets and reading config information.
    - `EcoSensor` initializes Sensor subclasses and handles collecting data from all initialized instances.
        - `Sensor` is a virtual class that is used as a superclass for sensor classes that have the machinery for connecting to various sensor types. The instance of a `Sensor` subclass will be initialized for each of the sensors connected to the system, and through their shared inheritance, all the different subclasses can be placed in a common vector in the EcoSensor class and interfaced within the same way. Each Sensor subclass must implement the two functions `readDataToString()` and `sensorPresent()`.
        - `AnalogSensor` subclasses the Sensor class, and implements the functionality for interfacing with analog sensors.
        - `SDISensor` implements the functionality for connecting to SDI-12 sensors.
        - `FlowSensor` interfaces with flow meters were readings are based on pulse-counting.
        - `SPISensor` interfaces with sensors working on the SPI protocol.
    - EcoTimer provides timing features.

## Dependencies
To launch code and interface with the microprocessor, install the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and the extension to make it work with the [Teensy development boards](https://www.pjrc.com/teensy/loader.html).

The project relies on a number of Arduino libraries, including:
- [Arduino-SDI-12-Teensy-35](https://github.com/MacKals/Arduino-SDI-12-Teensy-35) for interfacing with SDI-12 sensors.
- [Arduino-LMIC](https://github.com/mcci-catena/arduino-lmic) for LoRaWAN communication
- [UBLOX](https://github.com/bolderflight/UBLOX) for GPS communication
- [Teensy Snooze](https://github.com/duff2013/Snooze) for low power mode
- (not used, refference) [FlowMeter](https://github.com/sekdiy/FlowMeter)

Before building the project, all these libraries need to be installed in the Arduino environment (download zip, extract and drag the folder to the Arduino/libraries directory on your computer).

## Addressing
- Each node has a unique address on the network. The addresses start at 0 and increments.
- It must be easy to correlate each sensor with the data it produces. This is done by giving each sensor a unique address depending on which header it is connected to on the node. The header has its address in silkscreen close to the header. This address is the digital pin number on the Teensy that is connected to its data-pin.

### SDI-12 device addresses
The SDI-12 protocol requires unique char addresses for devices on the same bus. This is fulfilled by placing each SDI-12 device on its own bus, making conflicts in addresses impossible. The system is capable of dynamically determining the address of sensors, so the user does not need to worry about this at all. However, to speed connection times up, use the default address of a device, or addresses 0 and 1 as these are the ones that will be checked first.

## Message Structure
The [ASCII format](https://www.arduino.cc/en/Reference/ASCIIchart) is used for encoding all data sent over LoRaWAN. That means that a simple hex-to-ascii conversion will give readable results from the message sent over LoRa. Further details about the transmission format are documented in the code files.
TODO: document message format

## Deployment Procedure
1. connect all sensors you want to use with the node
2. update copies of the [config files](/configurations) and place them on the Teensy SD card
