//  EcoNode.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECONODE_H
#define _ECONODE_H

#include "Configuration.hpp"
#include "EcoRadio.hpp"
#include "EcoSensor.hpp"
#include <list>

class EcoNode {

    EcoRadio radio;
    std::list<EcoSensor> sensors;

    EcoRadio radio();

public:

    EcoNode();
    std::list<EcoNode> getChildren();
    void pollSensor(EcoSensor s);

    void init() {
        radio.init();
    }

    void printRegisters() {
        radio.printRegisters();
    }

    void execute() {
        //
    }
};

#endif
