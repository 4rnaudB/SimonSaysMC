#ifndef UTILS_H
#define UTILS_H
////////////// INCLUDES /////////////////////////
////////////////////////////////////////////////

#include <Arduino.h>

////////////////////////////////////////////////
////////////////////////////////////////////////

/////////////////// GLOBAL VARIABLES /////////////////////
//////////////////////////////////////////////////////////
enum ButtonColor : int {
  BLUE,
  RED,
  GREEN,
  YELLOW,
  NUM_BUTTONS // This automatically gets the value 4
};

extern const int BUTTON_PINS[NUM_BUTTONS];
extern const int LED_PINS[NUM_BUTTONS];
extern const int TONES[NUM_BUTTONS];

extern const int BUZZER_PIN;
extern const int TONE_DURATION_MS;

extern long lastDebounceTime[NUM_BUTTONS];

extern int lastButtonState[NUM_BUTTONS];

extern int buttonReading[NUM_BUTTONS];

extern const long DEBOUNCE_DELAY;

// Sequence params
struct Node {
  int value;
  Node* next;
};

extern Node* head;
extern volatile int nodeSize;

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

/////////////////// FUNCTIONS ////////////////////////////
//////////////////////////////////////////////////////////

void setup_pins_and_buttons();

int check_buttons();

void welcome_melody();

void play_sequence();

void reset_sequence();

void activate_color(int color, int timeDelay);

void validate_sequence();

void play_game_over();

#endif
