//  EcoTimer.hpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

// methods to provide sconsistent time-keeping across unit


#ifndef ECOTIMER_H
#define ECOTIMER_H

#include <TimeLib.h> // sync RTC
#include <Arduino.h>

#include "Configuration.hpp"

class EcoTimer {

    time_t timerTime = 0; // length of timer (s) + start time (RTC, s)

public:
    bool startTimer(uint16_t time); // s, delay time, false if already set
    bool timerDone();
    uint8_t minSecondsLeft(EcoTimer t);
    uint8_t minMinutesLeft(EcoTimer t);
    uint8_t minHoursLeft(EcoTimer t);
};

#endif
