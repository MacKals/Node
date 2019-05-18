//  Radio.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoRadio.hpp"


// little-endian format (least-significant-byte)
// LoRaWAN Connection Parameters
static u1_t APPEUI[8] = { 0x39, 0xA7, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 }; // little-endian
static u1_t DEVEUI[8] = { 0x5B, 0xF5, 0xF4, 0xFB, 0x08, 0x04, 0xBF, 0x00 }; // little-endian
static u1_t APPKEY[16] = { 0xF2, 0x81, 0xF2, 0x91, 0x2E, 0x5B, 0x79, 0x5B, 0x08, 0x18, 0x8B, 0xDF, 0x29, 0xBD, 0x98, 0xB4 }; // big-endian

void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}

volatile bool joined = false;
volatile bool _transmitSuccessfull = false;
volatile ev_t _lastEvent = EV_TXSTART;
// lef = little endian format, reverse order
void stringToHex(String s, u1_t *arr, bool lef = false) {
    for (uint8_t i = 0; i < s.length(); i += 2) {
        u1_t d = strtol(s.substring(i,i+2).c_str(), 0, 16);

        if (lef) arr[(s.length()-i)/2-1] = d;
        else arr[i/2] = d;
    }
}

void printArray(u1_t *arr, uint8_t size) {
    for (uint8_t i = 0; i< size; i++ ) {
        Serial.print(arr[i], HEX);
        PRINT(" ");
    }
    PRINTLN();
}

void EcoRadio::init(EcoSD sd) {

    // set LoRaWAN parameters from ini
    vector<String> lora = sd.getLoRaWANFromConfig();
	setLoRaParameters(lora[0], lora[1], lora[2]);


    SPI.setSCK(RFM95_CLK); // update clock pin

    // LMIC init.
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    // Disable link-check mode and ADR, because ADR tends to complicate testing.
    LMIC_setLinkCheckMode(0);
    // Set the data rate to Spreading Factor 7.  This is the fastest supported rate for 125 kHz channels, and it
    // minimizes air time and battery power. Set the transmission power to 14 dBi (25 mW).
    LMIC_setDrTxpow(DR_SF7,14);
    // in the US, with TTN, it saves join time if we start on subband 1 (channels 8-15). This will
    // get overridden after the join by parameters from the network. If working with other
    // networks or in other regions, this will need to be changed.
    LMIC_selectSubBand(1);
}


void EcoRadio::setLoRaParameters(String deveui, String appeui, String appkey) {

    // test for valid key lengths
    if (deveui.length() != 16) {
        PRINTLN("Wrong length deveui: " + deveui);
        return;
    } else if (appeui.length() != 16) {
        PRINTLN("Wrong length appeui: " + appeui);
        return;
    } else if (appkey.length() != 32) {
        PRINTLN("Wrong length appkey: " + appkey);
        return;
    }

    // update key strings
    stringToHex(deveui, DEVEUI, true);
    stringToHex(appeui, APPEUI, true);
    stringToHex(appkey, APPKEY);

    // print updated values
    PRINT("New DEVEUI: ");
    printArray(DEVEUI,8);
    PRINT("New APPEUI: ");
    printArray(APPEUI,8);
    PRINT("New APPKEY: ");
    printArray(APPKEY,16);
}


// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 24,
    .dio = {28, 29, 30},
};


void onEvent (ev_t ev) {
    PRINT(os_getTime());
    PRINT(": \t");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            PRINTLN(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            PRINTLN(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            PRINTLN(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            PRINTLN(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            PRINTLN(F("EV_JOINING"));
            break;
        case EV_JOINED:
            PRINTLN(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              PRINT("netid: ");
              PRINTLN2(netid, DEC);
              PRINT("devaddr: ");
              PRINTLN2(devaddr, HEX);
              PRINT("artKey: ");
              for (uint i=0; i<sizeof(artKey); ++i) {
                PRINT2(artKey[i], HEX);
              }
              PRINTLN("");
              PRINT("nwkKey: ");
              for (uint i=0; i<sizeof(nwkKey); ++i) {
                PRINT2(nwkKey[i], HEX);
              }
              PRINTLN("");
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
	        // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            joined = true;
            break;

        case EV_RFU1:
             PRINTLN(F("EV_RFU1"));
             break;
        case EV_JOIN_FAILED:
            PRINTLN(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            PRINTLN(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            PRINTLN(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              PRINTLN(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              PRINTLN(F(" bytes of payload"));
            }
            _transmitSuccessfull = true;
            // Schedule next transmission
            // os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            PRINTLN(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            PRINTLN(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            PRINTLN(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            PRINTLN(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            PRINTLN(F("EV_LINK_ALIVE"));
            break;
        case EV_SCAN_FOUND:
        	PRINTLN(F("EV_SCAN_FOUND"));
        	break;
        case EV_TXSTART:
            PRINTLN(F("EV_TXSTART"));
            break;
        default:
            PRINT(F("Unknown event: "));
            PRINTLN((unsigned) ev);
            break;
    }

    _lastEvent = ev;
}


bool EcoRadio::send(String s) {
	uint16_t length = s.length();
	uint8_t sendArray[length];
	for (uint16_t i = 0; i < length; i++) {
		sendArray[i] = (uint8_t) s.charAt(i);
	}

	if (!ready()) {
        PRINT(os_getTime());
        PRINT(": \t");
		    PRINTLN(F("OP_TXRXPEND, not sending"));
        return false;
	}

    // Prepare upstream data transmission at the next possible time.
	LMIC_setTxData2(1, sendArray, length, 0);
    _transmitSuccessfull = false;
	return true;
}

bool EcoRadio::ready() {
    // Check if there is not a current TX/RX job running
    return !(LMIC.opmode & OP_TXRXPEND);
}

bool EcoRadio::transmitting() {
    switch (_lastEvent) {
        case EV_SCAN_TIMEOUT:
        case EV_REJOIN_FAILED:
        case EV_JOIN_FAILED:
        case EV_RESET:

        case 20:
        case EV_TXCOMPLETE:
            return false;
    }

    return true;
}

bool EcoRadio::transmitSuccessfull() {
    return _transmitSuccessfull;
}
