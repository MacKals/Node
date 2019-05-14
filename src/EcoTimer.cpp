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
<<<<<<< HEAD
	return (Teensy3Clock.get() > timerTime); 
	return false;
=======
	return Teensy3Clock.get() > timerTime);
>>>>>>> 68ac6ba591d205451e1aecd0e9807d183a0a11a2
}
