# Codebase for UBC Ecohydrology Sensor Nodes

The code in this repository is made to provide a flexible solution for connecting sensors to a [LoRa based sensor node](https://github.com/UBCecohydro/AgWaterMon_CAD). The code is developed around the Teensy 3.5 microcontroller development board.

- Launch the Node.ino file in the Arduino IDE to compile the library.
- The class EcoNode is launched as the top level system manager, performing scheduling and coordinating the funcitonality of the system. Functionality is encapsulated in subclasses in the following manner:
    - `EcoRadio` interfaces with the LoRa module and LoRaWAN communication protocol.
    - `EcoSD` interfaces with the file system on the SD card. This includes buffering, storage and retrival of dropped packets and reading config information.
    - `EcoSensor` initializes Sensor subclasses and handles collecting data from all initialized instances.
        - `Sensor` is a virtual class that is used as a superclass for sensor classes that have the machinery for connecting to various sensor types. The instance of a `Sensor` subclass will be initialized for each of the sensors connected to the system, and through their shared inheritance, all the different subclasses can be placed in a common vector in the EcoSensor class and interfaced with in the same way. Each Sensor subclass must inplement the two functions `readDataToString()` and `sensorPresent()`.
        - `AnalogSensor` subclasses the Sensor class, and implements the funcitonality for interfacing with analog sensors.
        - `SDISensor` implements the functionality for connecting to SDI-12 sensors.
        - `FlowSensor` interfaces with flow meters were readings are based on pulse-counting.
        - `SPISensor` interfaces with sensors working on the SPI protocol.
    - EcoTimer provides timing features.

## Dependencies
To launch code and interface with the microprocessor, install the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and the extension to make it work with the [Teensy development boards](https://www.pjrc.com/teensy/loader.html).

The project relies on a number of Arduino libraries, including:
- [Arduino-SDI-12-Teensy-35](https://github.com/MacKals/Arduino-SDI-12-Teensy-35) for interfacing with SDI-12 sensors.
- [Arduino-LMIC](https://github.com/mcci-catena/arduino-lmic) for LoRaWAN communication
- (not used, refference) [FlowMeter](https://github.com/sekdiy/FlowMeter)


## Addressing
Each sensor node will have an App Key and EUI information to fascilitate encryted communication with The Things Network. The address can be loaded onto a SD card, and will be read into memory on startup.

TODO: Spesify file and format for this configuration.

The [ASCII format](https://www.arduino.cc/en/Reference/ASCIIchart) is used for encoding characters.


## Sensor addresses
Each sensor will need an associated address that is unique for each node so that data can be decoded properly. The address of a sensor is determined by which digital input on the Teensy it is connected to. Each terminal block is connected to a different digital input, and this address is clearly printed in silkscreen next to the connector.

### SDI-12 device addresses

The SDI-12 protocol requires unique char addresses for devices on the same bus. This is fulfilled by placing each SDI-12 device on its own bus, making conflicts in addresses impossible. The system is capable of dynamically determining the address of sensors, so the user does not need to worry about this at all. However, to speed connection times up, use the dafult addres of a device, or addresses 0 and 1 as these are the ones that will be ckecked first.

#### Configuring SDI-12 device addresses
TODO: make instructions

## Supported communication protocols
- SDI-12 (6 headers)
- Analog single ended (6 headers)
- Analog double ended (2 headers)
- SPI (2 headers)
- UART (2 headers)

### Identification procedure
1. configure node to work with The Things Network and physically label the node
2. configure SDI-12 sensor addresses and label
3. connect all sensors, label all non SDI-12 sensors and take note of configuration
4. configure


Before building the project, all these libraries need to be installed in the Arduino environment (download zip, extract and drag folder to the Arduino/libraries directory on your computer).

## Software TODO
- Add CO2 sensor support
- Fix file bug

## ECAD TODO
- Include header for config/reset type jumpers?
- Include headers to expose more IO?
