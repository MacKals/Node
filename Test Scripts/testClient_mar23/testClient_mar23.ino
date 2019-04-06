#include <SPI.h>
#include <RH_RF95.h>

// Create instance of the radio driver

#define RFM95_CS  10 // slave select
#define RFM95_RST 24 // reset
#define RFM95_INT 28 // interupt
#define RFM95_SCK 14 // clock, not primary

#define LED 13

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    SPI.setSCK(RFM95_SCK);
    Serial.begin(9600);
    delay(1000);
    Serial.println("initializing");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
    
    if (!rf95.init()) Serial.println("init failed");
    rf95.setFrequency(RF95_FREQ);
}

bool led = false;

void loop() {
  Serial.println("Sending to rf95_server");
  
  // Send a message to rf95_server
  uint8_t data[] = "Hello World!";
  digitalWrite(LED, led);
  led = !led;

  rf95.send(data, sizeof(data));
  
  Serial.println("Waiting for reply...");
  
  rf95.waitPacketSent();
  // Now wait for a reply
  
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  { 

    // Should be a reply message for us now   
   if (rf95.recv(buf, &len))
  {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }

  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(400);
}
