//  Radio.cpp
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

#include "EcoRadio.hpp"

//
// For normal use, we require that you edit the sketch to replace FILLMEIN
// with values assigned by the TTN console. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN to a non-
// working but innocuous value.
//
#ifdef COMPILE_REGRESSION_TEST
# define FILLMEIN 0
#else
# warning "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
# define FILLMEIN (#dont edit this, edit the lines that use FILLMEIN)
#endif



// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8]={ 0x39, 0xA7, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8]={ 0x5B, 0xF5, 0xF4, 0xFB, 0x08, 0x04, 0xBF, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = { 0xF2, 0x81, 0xF2, 0x91, 0x2E, 0x5B, 0x79, 0x5B, 0x08, 0x18, 0x8B, 0xDF, 0x29, 0xBD, 0x98, 0xB4 };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}


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
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            PRINT(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            PRINT(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            PRINT(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            PRINT(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            PRINT(F("EV_JOINING"));
            break;
        case EV_JOINED:
            PRINT(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("artKey: ");
              for (int i=0; i<sizeof(artKey); ++i) {
                Serial.print(artKey[i], HEX);
              }
              Serial.println("");
              Serial.print("nwkKey: ");
              for (int i=0; i<sizeof(nwkKey); ++i) {
                Serial.print(nwkKey[i], HEX);
              }
              Serial.println("");
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
	    // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;

        case EV_RFU1:
             PRINT(F("EV_RFU1"));
             break;
        case EV_JOIN_FAILED:
            PRINT(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            PRINT(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            PRINT(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              PRINT(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              PRINT(F(" bytes of payload"));
            }
            // Schedule next transmission
            // os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            PRINT(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            PRINT(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            PRINT(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            PRINT(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            PRINT(F("EV_LINK_ALIVE"));
            break;
        case EV_SCAN_FOUND:
        	PRINT(F("EV_SCAN_FOUND"));
        	break;
        case EV_TXSTART:
            PRINT(F("EV_TXSTART"));
            break;
        default:
            Serial.print(F("Unknown event: "));
            PRINT((unsigned) ev);
            break;
    }
}


void EcoRadio:: send(String s) {
	uint16_t length = s.length();
	uint8_t sendArray[length];
	for (uint16_t i = 0; i < length; i++) {
		sendArray[i] = (uint8_t) s.charAt(i);
	}

	// Check if there is not a current TX/RX job running
	if (LMIC.opmode & OP_TXRXPEND) {
		PRINT(F("OP_TXRXPEND, not sending"));
	} else {
		// Prepare upstream data transmission at the next possible time.
		LMIC_setTxData2(1, sendArray, length, 0);
		PRINT(F("Packet queued"));
	}
	// Next TX is scheduled after TX_COMPLETE event.

}
