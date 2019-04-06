//  EcoNode.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-29

#include "EcoNode.hpp"
#include <list>


//Constructor
EcoNode::EcoNode() {
    parentAddress = 0; // will need to find this
    nodeAddress = getAddress();
}


//
void EcoNode::init(){ //start radio
   bool parentFound = radio.init(); //power up and get a parent

   if (parentFound) {
     //parentAddress = radio.getParentAddress();
   }

   // get connected sensors
   //SDI-12
   
}



void EcoNode::collectSensorData(){ // collect from all

}

void EcoNode::pollSensor(EcoSensor s){

}

// can't change address live
// value between 1 and 63
int EcoNode::getAddress(){
   return 1*digitalRead(DIP0) + 2*digitalRead(DIP1) + 4*digitalRead(DIP2) + 8*digitalRead(DIP3) + 16*digitalRead(DIP4) + 32*digitalRead(DIP5);
}

std::list<EcoSensor> EcoNode::getSensors(){
   return this->sensors;
}

std::list<EcoNode> EcoNode::getChildren(){
   return this->children;
}
