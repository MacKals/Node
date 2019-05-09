
# Configuration files
## LoRaWAN
To establish a connection with the LoRaWAN server, you need to register the node as a new device on The Things Network. Once you have set the new device up, there is three keys that will have to be transferred to the node: Device EUI, Application EUI and App Key. You can do this by placing a file on the SD card of the node.

### Instrucitons:
- Place the file in the root directory of the SD card.
- Name the file "lorawan.txt"
- Paste the three values (Device EUI, Application EUI and App Key) in that order, by using the copy button of the strings on the TTN website. Do not use little endian format for any of the parameters, copy them exatly as shown.
- Separate the keys by newline.

You should end up with a file that only contains strings looking somehting like this:
```
00749A95AC2874C3
7063D57E3006A734
22F2C4EEA5832B46352375752C1853AA
```

## Sensor Configuration
In order to tell the system what data is coming from which sensor, a file called `sensors.txt` is used.

- `xx` - header number (excluding the D)
- `min` - min analog voltage the sensor can output in volts
- `max` - max analog voltage the sensor can output in volts
- `toMin` - desired reading when the value is min
- `toMax` - desired reading when the value is max
- `title` - title used to describe information, preferably including units. The title can have spaces

### Analog sensors
`A,xx,min,max,toMin,toMax,title`
### SDI-12 sensors
`S,xx,title1,title2,...`
Add as many titles as there are data fileds on the SDI sensor
### PWM sensors
`P,xx,toMin,toMax,title`
### Flow sensors
`F,xx,???` TODO: how to decode ticks to


## Common sensors and their configurations
Below is a list of sensors used and how they should/could be configured. Make sure to update the `xx` to correspond to the pin you have connected the sensor to.

TODO: expand list, include images

Meter GS3: `S, xx, Dielectric Permittivity, Temperature (C), Electrical Conductivity (uS/cm)`
SoilWatch 10: `A, xx, 0, 3, 0, 100, Moisture Level`
