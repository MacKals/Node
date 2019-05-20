
# Configuration files
Nodes use a single INI file (_config.ini_) for storing details that are unique to the given node. This includes the following information:

1. LoRaWAN connection details
2. Sensors connected
3. Current deployment information (optional)

This file must be edited to reflect the given node's details and uploaded to its associated microSD card onboard the unit.
Details about the structure of INI files can be found on [Wikipedia](https://en.wikipedia.org/wiki/INI_file).

## LoRaWAN
To establish a connection with the LoRaWAN server, the node must be registered as a new device on The Things Network. Once the device is created, there are three keys that will have to be transferred to the node: **Device EUI**, **Application EUI** and **App Key**. Update the fields in the config file with these keys. Simply copy and paste the keys as they appear, rather than converting to little endian, etc.

Details about device registration can be found at [The Things Network](https://www.thethingsnetwork.org/docs/devices/registration.html).

## Sensors Connected
Each sensor connected to the node (that collects data desired by the user) must be documented in _config.ini_. There are two fields required: the serial number (internal numbering scheme) and the sensor type. The supported sensor types have the following keywords:

- SDI-12
- Flow
- Analog
- PWM

The serial number associated with the sensor should be visible on the sensor itself on a custom asset label.

## Current deployment information (optional) TODO
While currently under development, there will exist a field in _config.ini_ for describing the purpose of the node for a given deployment.
