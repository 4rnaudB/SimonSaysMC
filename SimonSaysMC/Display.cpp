#include "Display.h"


 Display::Display(): lcd(0x27, LCD_COLS, LCD_ROWS)
    {
      lcd.clear();
      lcd.init();
      lcd.backlight();
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Simon says");
      lcd.setCursor(0, 1);
      lcd.print("loading...");
    }
    
void Display::write_text(const char* text)
{
  lcd.clear();

  // Check if the user provided a '\n' for manual line break
  const char* newline = strchr(text, '\n');

  if (newline) {
    // --- Case 1: Explicit newline provided ---
    int line1_len = newline - text;
    int line2_len = strlen(newline + 1);

    char line1[17] = {0};
    char line2[17] = {0};

    strncpy(line1, text, min(line1_len, LCD_COLS));
    strncpy(line2, newline + 1, min(line2_len, LCD_COLS));

    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
  } 
  else {
    // --- Case 2: Auto-wrap long text ---
    int len = strlen(text);

    if (len <= LCD_COLS) {
      // Fits on one line
      lcd.setCursor(0, 0);
      lcd.print(text);
    } 
    else if (len <= LCD_COLS * 2) {
      // Try to find a space near the middle to split nicely
      int splitPos = -1;
      for (int i = LCD_COLS; i >= 0; i--) {
        if (text[i] == ' ') {
          splitPos = i;
          break;
        }
      }

      if (splitPos == -1) splitPos = LCD_COLS; // fallback: hard split

      char line1[17] = {0};
      char line2[17] = {0};

      strncpy(line1, text, splitPos);
      strncpy(line2, text + splitPos + 1, LCD_COLS);

      lcd.setCursor(0, 0);
      lcd.print(line1);
      lcd.setCursor(0, 1);
      lcd.print(line2);
    } 
    else {
      // Too long — truncate and indicate overflow
      char truncated[17] = {0};
      strncpy(truncated, text, LCD_COLS - 3);
      strcat(truncated, "...");
      lcd.setCursor(0, 0);
      lcd.print(truncated);
    }
  }
}
