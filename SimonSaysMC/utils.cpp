#include "utils.h"
#include <avr/io.h>          // includes <avr/iom328p.h>
#include <avr/interrupt.h>   // defines ISR() macro


/////////////////// GLOBAL VARIABLES /////////////////////
//////////////////////////////////////////////////////////
const int BUTTON_PINS[NUM_BUTTONS] = {7, 6, 5, 4};
const int LED_PINS[NUM_BUTTONS]    = {8, 9, 10, 11};
const int TONES[NUM_BUTTONS]       = {262, 294, 330, 349};
const uint8_t BUTTONS_BITS         = 0b11110000;

const int BUZZER_PIN = 3;
const int TONE_DURATION_MS = 150;

long lastDebounceTime[NUM_BUTTONS] = {0, 0, 0, 0};

int lastButtonState[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH};

int buttonReading[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH};


const long DEBOUNCE_DELAY = 50; // ms

volatile uint8_t pinD_state = 0b11110000;
volatile bool confirm_sequence = false;
volatile bool button_pressed = false;
//static volatile int count = 0;
uint8_t lvl = 0;

// Display
Display* lcd_display = nullptr;

// EEPROM MEMORY
const int EASY_SLOT_1 = 0;
const int EASY_SLOT_2 = 1;
const int HARD_SLOT_1 = 2;
const int HARD_SLOT_2 = 3;

// Game OVER SOUND
const int game_over_freqs[] = {
  392,  // Note 1 (Highest)
  370,  // Note 2
  349,  // Note 3
  330,  // Note 4
  311   // Note 5 (Lowest/Final)
};
const int NUM_NOTES = sizeof(game_over_freqs) / sizeof(game_over_freqs[0]);
///////////////

//Sequence params
Node* head = nullptr;
volatile int nodeSize = 0;
 
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


//////////////// ISR ////////////////////////
////////////////////////////////////////////
ISR ( PCINT2_vect )
{
  if (pinD_state < (PIND & BUTTONS_BITS))
  {
    confirm_sequence = true;
    PORTB &= BUTTONS_BITS;
    return;
  }
  button_pressed = true;
  pinD_state = PIND & BUTTONS_BITS;
}

////////////////////////////////////////////
///////////////////////////////////////////

void choose_level()
{
  while (true)
  {
    lcd_display->write_text("Easy: Blue\nHard: Red");
    enable_portD_Interrupts();

    while (!button_pressed)
    {
      delay(100);
    }

    disable_portD_interrupts();
    button_pressed = false;

    if ((~pinD_state & 0xF0) == 0b10000000) // blue button
    {
      lvl = 1;
      break;
    }
    else if ((~pinD_state & 0xF0) == 0b01000000) // red button
    {
      lvl = 2;
      break;
    }
    else // wrong button pressed
    {
      lcd_display->write_text("Wrong button pressed");
      delay(1000);
      // Loop repeats, prompting user again
    }
  }

   // Reset portD
   pinD_state = BUTTONS_BITS;

   // Wait 2 seconds before starting sequence
   lcd_display->write_text("Let's play...");
   delay(2000);
}


void load_screen()
{
  lcd_display = new Display();
}

void setup_pins_and_buttons()
{
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
}

void enable_portD_Interrupts() {
  cli();
  PCICR |= (1 << PCIE2);
  PCIFR = 0b00000111;
  PCMSK2 |= (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23);
  sei();
}

void disable_portD_interrupts() {
  cli();
  PCICR &= ~(1 << PCIE2);
  PCIFR = 0b00000111;
  sei();
}

void check_buttons()
{
  enable_portD_Interrupts();
  while (!confirm_sequence)
  {
    if (button_pressed)
    {
      button_pressed = false;
      int frequency = 0, count = 0;
      for (uint8_t i = 4; i < 8; i++)
      { 
        if (!(pinD_state & (1 << i)))
        {
          frequency += TONES[i - 4];
          count++;
        }
      }
      if (count == 0)
        continue;
      PORTB = (~pinD_state >> 4) & 0x0F;
      tone(BUZZER_PIN, frequency / count);
    }
    delay(1);
  }

  disable_portD_interrupts();
  confirm_sequence = false;
  noTone(BUZZER_PIN);
}

void welcome_melody()
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
      activate_color((1 << i), 100);
    }
}

void play_sequence()
{
  static int nextColor;
  if (lvl == 1) // only 1 color at a time
    nextColor = 1 << random(0, NUM_BUTTONS);
  else // lvl 2, can be between 1 and 4 colors
    nextColor = random(1, 15);
    
  Node* newNode = new Node{nextColor, nullptr};
  
  if (head == nullptr) {
    activate_color(nextColor, 300);
    head = newNode;
    nodeSize = 1;
    return;
  }

  Node* current = head;
  while (current) { // Checks if the CURRENT node exists
    activate_color(current->value, 300);
    
    if (current->next == nullptr) { // Check if this is the LAST node
        break; // Stop playing the sequence once the last one is done
    }
    
    current = current->next;
}
  
  activate_color(nextColor, 300);
  current->next = newNode;
  nodeSize++;
}

void reset_sequence()
{
  Node* current = head;
  while (current) {
    Node* nextNode = current->next;
    delete current;
    current = nextNode;
  }
  head = nullptr;
  nodeSize = 0;
}

void activate_color(uint8_t color, int timeDelay)
{
  PORTB |= color;

  int frequency = 0;
  int count = 0;
  // Loop through bits 0-4 to find which color is activated
  for (uint8_t i = 0; i < 4; i++) {
    if (color & (1 << i)) {
      frequency += TONES[i];
      count++;
    }
  }
  frequency /= count;
  tone(BUZZER_PIN, frequency);
  
  delay(timeDelay);
  
  PORTB &= ~color;
  noTone(BUZZER_PIN);
  delay(timeDelay);
}

bool validate_sequence()
{
  int pressedButton;
  Node* nextNode = head;

  for (int i = 0; i < nodeSize; i++)
  {
    check_buttons();
    
    // Buttons are input pull up --> need to reverse the buttons bits + shift 4 bits to fit with the LED pins
    uint8_t pressedButtons = (~pinD_state & BUTTONS_BITS) >> 4; 
    if (pressedButtons != nextNode->value)
    {
      pinD_state = BUTTONS_BITS;
      play_game_over();
      reset_sequence();
      return false;
    }

    pinD_state = BUTTONS_BITS;
    
    nextNode = nextNode->next;
    delay(10);
  }

  return true;
  
}

void play_game_over()
{
  for (int i = 0; i < NUM_NOTES; i++) {
    // Play the tone for a set duration
    tone(BUZZER_PIN, game_over_freqs[i], 150);
    
    // Wait for the duration of the note plus a small pause
    delay(170);
  }
  
  // Ensure the sound is fully off after the sequence
  noTone(BUZZER_PIN);
  delay(1000);
}

void save_high_score(uint8_t score)
{
  int high_score_address;
  int high_score;
  if ( lvl == 1)
  {
    if ((uint8_t)EEPROM.read(EASY_SLOT_1) < (uint8_t)EEPROM.read(EASY_SLOT_2))
    {
      high_score_address = EASY_SLOT_1;
      high_score = EEPROM.read(EASY_SLOT_2);
    }
    else
    {
      high_score_address = EASY_SLOT_2;
      high_score = EEPROM.read(EASY_SLOT_1);
    }
  }
  else // lvl = 2
  {
    if ((uint8_t)EEPROM.read(HARD_SLOT_1) < (uint8_t)EEPROM.read(HARD_SLOT_2))
    {
      high_score_address = HARD_SLOT_1;
      high_score = EEPROM.read(HARD_SLOT_2);
    }
    else
    {
      high_score_address = HARD_SLOT_2;
      high_score = EEPROM.read(HARD_SLOT_1);
    }
  }
  
  if (score > high_score)
  {
     EEPROM.update(high_score_address, score);
     high_score = score;
  }
   
  char buffer[33] = "";
  sprintf(buffer, "score:%d / HS:%d\nBlue = restart", score, high_score);
  lcd_display->write_text(buffer);
  while (digitalRead(BUTTON_PINS[BLUE])) {delay(100);}
  delay(100);
}
