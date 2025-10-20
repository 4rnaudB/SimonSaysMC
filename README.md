# SimonSaysMC 🎮

A classic Simon Says memory game built for Arduino microcontrollers! Test your memory by repeating increasingly complex sequences of colors and sounds.

## 🎯 Features

- **Two Difficulty Levels**: Choose between Easy and Hard modes
- **Visual & Audio Feedback**: LEDs and buzzer provide engaging gameplay
- **LCD Display**: 16x2 I2C LCD shows game status and scores
- **High Score Tracking**: Stores best scores in EEPROM memory
- **Debounced Button Input**: Reliable button detection with interrupt-driven I/O
- **Dynamic Sequence Generation**: Random sequences grow with each successful round

## 🔧 Hardware Requirements

### Schematic
📄 **Complete circuit schematic available**: [`SimonSaysMC_schematic.pdf`](SimonSaysMC_schematic.pdf)

### Components
- **Arduino Board** (ATmega328P compatible)
- **4 Colored Buttons**: Blue, Red, Green, Yellow (pins 7, 6, 5, 4)
- **4 LEDs**: Matching colors (pins 8, 9, 10, 11)
- **Buzzer**: Active or passive buzzer (pin 3)
- **16x2 LCD Display**: I2C interface (SDA/SCL)
- **Resistors**: Appropriate values for LEDs and pull-up configuration

### Pin Configuration
| Component | Pin(s) |
|-----------|--------|
| Blue Button | D7 |
| Red Button | D6 |
| Green Button | D5 |
| Yellow Button | D4 |
| Blue LED | D8 |
| Red LED | D9 |
| Green LED | D10 |
| Yellow LED | D11 |
| Buzzer | D3 |
| LCD (I2C) | A4 (SDA), A5 (SCL) |

## 📦 Dependencies

Install the following libraries through the Arduino IDE Library Manager:

- **Wire** (built-in) - I2C communication
- **LiquidCrystal_I2C** - LCD control
- **EEPROM** (built-in) - High score storage

## 🚀 Getting Started

1. **Clone or download** this repository
2. **Check the schematic** - Review `SimonSaysMC_schematic.pdf` for wiring details
3. **Install required libraries** in Arduino IDE
4. **Wire up the hardware** according to the schematic
5. **Open** `SimonSaysMC.ino` in Arduino IDE
6. **Upload** to your Arduino board
7. **Play!** Select difficulty and start memorizing sequences

## 🎮 How to Play

1. **Power on** the Arduino - you'll hear a welcome melody
2. **Choose difficulty**:
   - Press **Blue** for Easy mode
   - Press **Red** for Hard mode
3. **Watch the sequence**: The game will light up LEDs and play tones
4. **Repeat the sequence**: Press the buttons in the same order
5. **Score**: Each successful round adds 1 point and extends the sequence
6. **Game Over**: If you make a mistake, the game ends and shows your score

## 📁 Project Structure

```
SimonSaysMC/
├── README.md
├── SimonSaysMC_schematic.pdf  # Circuit schematic
└── SimonSaysMC/
    ├── SimonSaysMC.ino        # Main game logic and loop
    ├── Display.cpp             # LCD display implementation
    ├── Display.h               # LCD display class definition
    ├── utils.cpp               # Game utilities and functions
    └── utils.h                 # Constants, globals, and function declarations
```

## 🛠️ Code Features

- **Interrupt-driven button handling** using Pin Change Interrupts (PCINT)
- **Linked list** data structure for sequence management
- **Debouncing algorithm** for reliable button detection
- **EEPROM storage** for persistent high scores (separate slots for each difficulty)
- **Object-oriented design** with Display class abstraction

## 📝 License

This project is open source and available for educational purposes.
