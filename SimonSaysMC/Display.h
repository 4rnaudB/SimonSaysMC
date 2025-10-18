#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display
{
  private:
    LiquidCrystal_I2C lcd;  // make it a member so it persists
    static const int LCD_COLS = 16;
    static const int LCD_ROWS = 2;

  public:
    Display();
    void write_text(const char* text);
};

#endif
