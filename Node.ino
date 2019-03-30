//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoNode.hpp"

#include <SPI.h>
#include <SDI12.h>
#include <RH_RF95.h>

EcoNode node;

void setup() {
  node = EcoNode(); // init the EcoNode object
  Serial.begin(9600);
}

void loop() {
   node.execute();
}
