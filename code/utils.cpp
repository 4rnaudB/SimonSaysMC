#include "utils.h"


/////////////////// GLOBAL VARIABLES /////////////////////
//////////////////////////////////////////////////////////
const int BUTTON_PINS[NUM_BUTTONS] = {7, 6, 5, 4};
const int LED_PINS[NUM_BUTTONS]    = {13, 12, 11, 10};
const int TONES[NUM_BUTTONS]       = {262, 294, 330, 349};

const int BUZZER_PIN = 3;
const int TONE_DURATION_MS = 150;

long lastDebounceTime[NUM_BUTTONS] = {0, 0, 0, 0};

int lastButtonState[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH};

int buttonReading[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH};

const long DEBOUNCE_DELAY = 50; // ms

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

void setup_pins_and_buttons()
{
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
}

int check_buttons()
{
  while(true)
  {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      
      int reading = digitalRead(BUTTON_PINS[i]);

      if (reading != buttonReading[i]) {
        // The button state has changed (it's bouncing or the press/release started), 
        // so reset the debouncing timer.
        lastDebounceTime[i] = millis();
      }
      
      if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
  
        // Check if the current reading is different from the *last known stable state*.
        // This is the core logic: only act if the stable state has changed.
        if (reading != lastButtonState[i]) {
          // Save the new stable state
          lastButtonState[i] = reading;
  
          if (lastButtonState[i] == LOW) {
            // Button is stable and pressed (LOW)
            digitalWrite(LED_PINS[i], HIGH);
            // Play a non-blocking tone
            tone(BUZZER_PIN, TONES[i]); 
          } else {
            // Button is stable and released (HIGH)
            digitalWrite(LED_PINS[i], LOW);
            // (Optional) Stop the tone if released before duration ends, 
            // though the tone() with duration usually handles this.
            noTone(BUZZER_PIN);
  
            return i;
          }
        }
      }
      // 5. Save the *current raw reading* for the next loop's state change detection
      buttonReading[i] = reading;
    }
  }
}

void welcome_melody()
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
      activate_color(i, 100);
    }
}

void play_sequence()
{
  int nextColor = random(0, NUM_BUTTONS);
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

void activate_color(int color, int timeDelay)
{
  digitalWrite(LED_PINS[color], HIGH);
  tone(BUZZER_PIN, TONES[color]);
  delay(timeDelay);
  digitalWrite(LED_PINS[color], LOW);
  noTone(BUZZER_PIN);
  delay(timeDelay);
}

void validate_sequence()
{
  int pressedButton;
  Node* nextNode = head;

  for (int i = 0; i < nodeSize; i++)
  {
    pressedButton = check_buttons();
    
    if (pressedButton != nextNode->value)
    {
      play_game_over();
      reset_sequence();
      return;
    }

    nextNode = nextNode->next;
  }
  
}

void play_game_over() {
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
