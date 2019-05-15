//  Node.ino
//  UBC Ecohydrology Node
//
//  Erik MacLennan, Morten Kals
//  2019-03-23

namespace std {
void __throw_bad_alloc() {
  Serial.println("Unable to allocate memory");
}
void __throw_length_error( char const*e ) {
  Serial.print("Length Error :"); Serial.println(e);
}
}

#include "EcoNode.hpp"
#include "Configuration.hpp"
EcoNode node;

void setup() {

    #ifdef DEBUG
        Serial.begin(115200);
        while (!Serial) {}
    #endif

    delay(200);

    node.init();
}

void loop() {
    node.loop();
}
