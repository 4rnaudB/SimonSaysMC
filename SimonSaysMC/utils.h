#ifndef UTILS_H
#define UTILS_H
////////////// INCLUDES /////////////////////////
////////////////////////////////////////////////

#include <Arduino.h>
#include "Display.h"
#include <EEPROM.h>
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

extern volatile uint8_t pinD_state;

extern uint8_t lvl;

extern Display* lcd_display;

// Sequence params
struct Node {
  uint8_t value;
  Node* next;
};

extern Node* head;
extern volatile int nodeSize;

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

/////////////////// FUNCTIONS ////////////////////////////
//////////////////////////////////////////////////////////
void choose_level();

void load_screen();

void setup_pins_and_buttons();

void enable_portD_Interrupts();

void disable_portD_interrupts();

void check_buttons();

void welcome_melody();

void play_sequence();

void reset_sequence();

void activate_color(uint8_t color, int timeDelay);

bool validate_sequence();

void play_game_over();

void save_high_score(uint8_t score);

void enableISR();

void disableISR();

#endif
