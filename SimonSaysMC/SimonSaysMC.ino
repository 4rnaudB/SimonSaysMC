///////// INCLUDES //////////////
////////////////////////////////
#include "utils.h"

void setup() {
  load_screen();
  setup_pins_and_buttons();
  randomSeed(analogRead(A0));
  Serial.begin(115200);
  welcome_melody();
  delay(500);
}

// --- Main Loop ---

void loop() {
  choose_level();

  while ( true )
  {
    play_sequence();
    if (!validate_sequence())
      break;
    delay(500);
  }
}
