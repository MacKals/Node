//  EcoTimer.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27


#include "EcoTimer.hpp"

bool EcoTimer::startTimer(uint16_t t) {
	if (!timerDone()) return false;

	timerTime = Teensy3Clock.get() + t;
	return true;
}

bool EcoTimer::timerDone() {
	return (Teensy3Clock.get() > timerTime); 
}
