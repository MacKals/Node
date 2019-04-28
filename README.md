# UBC Ecohydrology sensor nodes

## Addressing
Each sensor node will have an App Key and EUI information to fascilitate encryted communication with The Things Network. The address can be loaded onto a SD card, and will be read into memory on startup.

TODO: Spesify file and format for this configuration.

The [ASCII format](https://www.arduino.cc/en/Reference/ASCIIchart) is used for encoding characters.


## Sensor addresses
Each sensor will need an associated address that is unique for each node. The SDI-12 protocol requires unique char addresses for devices on the same bus. Due to the requirenments for manually changing addresses, and as this interferes with the header pin based addressing scheme, each header has a separate bus. All sensors will get addresses based on the terminal block they are connected to (see silkscreen next to relevant terminals).

Supported communication protocols
- SDI-12 (6 headers)
- Analog single ended (6 headers)
- Analog double ended (2 headers)
- SPI (2 headers)
- UART (2 headers)

This allows for reuse-it is possible to configure the analog sensor pins to operate as digital pins (if we do not go with a scheme of an external ADC).

Addressing scheme:
One uint8 as address corresponding to the digital pin the sensor is connected to. Some of the headers provide bus interfaces (SDI12, SPI), but due to the expansive IO of the system, these are assumed to each hold one device only. 


### Configuring SDI-12 device addresses
TODO: make instructions

### Identification procedure
1. configure node to work with The Things Network and physically label the node
2. configure SDI-12 sensor addresses and label
3. connect all sensors, label all non SDI-12 sensors and take note of configuration
4. configure


# Software TODO
-

# ECAD TODO
- Include header for config/reset type jumpers?
- Include headers to expose more IO?
