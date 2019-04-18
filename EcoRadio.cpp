//  Radio.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoRadio.hpp"

bool EcoRadio::sendDataWithReciept(String data, time_t time, uint8_t from, uint8_t to) {

    uint8_t firstOctet = B10000000; // expecting reciept
    String timeString = String((char*)(uint8_t*)&time);

    String header = String(firstOctet) + String(from) + String(to) + timeString;

    sendRawData(header + data);
    if (lora.waitAvailableTimeout(TIMEOUT_WAIT_TIME)) {
        String reply = readData();
        Serial.println("Recieved message=" + String(reply));

        const uint8_t* octetReply = reinterpret_cast<const uint8_t*>(&data[0]);
        if (octetReply[0] == firstOctet) return true;
        else Serial.println(octetReply[0]);
    }

    return false;
}
