//  EcoSensor.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#ifndef _ECOSENSOR_H
#define _ECOSENSOR_H

#include "Configuration.hpp"

enum SensorType {
    Serial12, //SDI-12
    Digital,
    Analog,
    Differential,
};

class EcoSensor {

  SensorType sensorType;
  uint8_t pin1;
  uint8_t pin2; // differential signals

public:

  SensorType getSensorType();
  double read();

private:

};

#endif
