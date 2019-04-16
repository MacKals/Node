//  EcoNode.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoNode.hpp"


//Constructor
EcoNode::EcoNode() {
}

void EcoNode::init() {

	this->sensors.init();
	sensors.attachAllSDI12();


   // bool parentFound = radio.init(); //power up and get a parent

   // if (parentFound) {
   //   parentAddress = radio.getParentAddress();
   // }

   // get connected sensors

}

// std::list<EcoSensor> EcoNode::getSensors(){
//    return this->sensors;
// }
//
// std::list<EcoNode> EcoNode::getChildren(){
//    return this->children;
// }

void EcoNode::collectSensorData(){ // collect from all

}

void EcoNode::pollSensor(EcoSensor s){

}

// can't change address live
int EcoNode::getAddress(){
   return 1*digitalRead(DIP0) + 2*digitalRead(DIP1) + 4*digitalRead(DIP2) + 8*digitalRead(DIP3) + 16*digitalRead(DIP4) + 32*digitalRead(DIP5);
}
