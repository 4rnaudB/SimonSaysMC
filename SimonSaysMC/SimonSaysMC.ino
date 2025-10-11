///////// INCLUDES //////////////
////////////////////////////////
#include "utils.h"

void setup() {
  setup_pins_and_buttons();
  randomSeed(analogRead(A0));
  Serial.begin(115200);
  welcome_melody();
  delay(500);
}

// --- Main Loop ---

void loop() {
  play_sequence();
  validate_sequence();
   delay(500);
  //check_buttons();
}
