///////// INCLUDES //////////////
////////////////////////////////
#include "utils.h"
#include <Wire.h>

void setup() {
  Wire.begin(); // For I2C communication with LCD
  load_screen(); // Init display
  setup_pins_and_buttons(); // Set LEDs to output and Buttons to INPUT_PULLUP
  randomSeed(analogRead(A0)); // generate random seed
  Serial.begin(115200); // Start serial communication for debugging purposes
  
  welcome_melody(); // Start up song
  delay(500);
}

// --- Main Loop ---

void loop() {
  choose_level();
  uint8_t score = 0;
  char buffer[17] = "";
  while ( true )
  {
    sprintf(buffer, "score: %d", score);
    lcd_display->write_text(buffer);
    delay(500);
    play_sequence();
    if (!validate_sequence())
      break;
    score++;
  }

  save_high_score(score);
}
