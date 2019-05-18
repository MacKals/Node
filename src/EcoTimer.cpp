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


uint8_t EcoTimer::minSecondsLeft(EcoTimer t) {
	time_t tMin = min(this->timerTime, t.timerTime);
	return max((tMin-Teensy3Clock.get()) % 60, 0);
}

uint8_t EcoTimer::minMinutesLeft(EcoTimer t) {
	time_t tMin = min(this->timerTime, t.timerTime);
	return max(((tMin-Teensy3Clock.get()) / 60) % 60, 0);
}

uint8_t EcoTimer::minHoursLeft(EcoTimer t) {
	time_t tMin = min(this->timerTime, t.timerTime);
	return max((tMin-Teensy3Clock.get()) / 3600, 0);
}
