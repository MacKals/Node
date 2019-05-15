//  AnalogSensor.h
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-04-27

#include "FlowSensor.h"


String FlowSensor::readDataToString() {
    String data = String(getAverageFlow()) + "," + String(getTotalFlow());
    reset();
    return data;
}

// AnalogSensor specific methods


/*
The following is a hack as it is not poassible to have an ISR call a class
member function. Instead, we use global funcitons that know which instance
they should call to increment.
*/

FlowSensor *interruptSubscribers[10] = {};
uint8_t subscriberCount = 0;

void isr0() { interruptSubscribers[0]->count++; }
void isr1() { interruptSubscribers[1]->count++; }
void isr2() { interruptSubscribers[2]->count++; }
void isr3() { interruptSubscribers[3]->count++; }
void isr4() { interruptSubscribers[4]->count++; }
void isr5() { interruptSubscribers[5]->count++; }
void isr6() { interruptSubscribers[6]->count++; }
void isr7() { interruptSubscribers[7]->count++; }
void isr8() { interruptSubscribers[8]->count++; }
void isr9() { interruptSubscribers[9]->count++; }

// Set up ISR for class instance
bool FlowSensor::init() {

    if (initialized) return true;

    // fund right isr function
    void (*isr)(void); // pointer to function
    switch (subscriberCount) {
        case 0: isr = isr0;
                break;
        case 1: isr = isr1;
                break;
        case 2: isr = isr2;
                break;
        case 3: isr = isr3;
                break;
        case 4: isr = isr4;
                break;
        case 5: isr = isr5;
                break;
        case 6: isr = isr6;
                break;
        case 7: isr = isr7;
                break;
        case 8: isr = isr8;
                break;
        case 9: isr = isr9;
                break;
        default:
            return false;
    }

    pinMode(pin, INPUT);
    interruptSubscribers[subscriberCount] = this;
    attachInterrupt(pin, isr, RISING);

    subscriberCount++;

    initialized = true;
    return true;
}


float FlowSensor::getTotalFlow() {
    return count;
}

float FlowSensor::getAverageFlow() {
    time_t deltaT = Teensy3Clock.get() - currentCountStartTime;
    return ((float) count) / deltaT;
}

void FlowSensor::reset() {
    // set up for next measurnment
    count = 0;
    currentCountStartTime = Teensy3Clock.get();
}

// assume this is the last instance
FlowSensor::~FlowSensor() {
    if (initialized) subscriberCount--;
}
