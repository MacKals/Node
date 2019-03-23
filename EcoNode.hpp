//  EcoNode.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#ifndef _ECONODE_H
#define _ECONODE_H

#include "Configuration.hpp"
#include "Radio.hpp"

class EcoNode {

    void initializeNavigator();
    void initializePickup(bool rightSidePickup);
    void initializeDropoff();

public:

    EcoNode() {
        // init things, RF, Name, get sensors that are connected from internet?

    }

};

#endif
