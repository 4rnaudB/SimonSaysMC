///////// INCLUDES //////////////
////////////////////////////////
#include "utils.h"

void setup() {
  setup_pins_and_buttons();
  randomSeed(analogRead(A0));
  welcome_melody();
//  Serial.begin(115200);
  delay(500);
}

// --- Main Loop ---

void loop() {
  play_sequence();
  validate_sequence();
   delay(500);
  //check_buttons();
}
