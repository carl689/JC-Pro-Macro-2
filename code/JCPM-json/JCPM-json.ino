#include <HID-Project.h>        // For keyboard and mouse HID functionality
#include <EEPROM.h>             // For storing configurations
#include <ArduinoJson.h>        // For parsing JSON configurations
#include <Adafruit_NeoPixel.h>  // For NeoPixel LED control
#include <Encoder.h>

// Structure to represent a button's configuration
struct ButtonConfig {
  char action[6];
  char keys[10];
  char modifiers[5];
  int x, y;
  int color[3];
  int colorOnPress[3];
};

// Structure to represent an encoder configuration
struct EncoderConfig {
  char action[6];       // Action type: "key", etc.
  char keys[10];         // Keys to be pressed (like "UpArrow" or "DownArrow")
  char modifiers[5];    // Modifiers (e.g., "Ctrl", "Shift")
};

EncoderConfig encoderConfigs[2]; // [0] for increment, [1] for decrement
ButtonConfig buttonConfigs[9];  // 9 configurable buttons

// General device settings
struct GeneralConfig {
  int defaultColor[3];  // Default color for all buttons if none specified  
};

GeneralConfig generalConfig; // Single global profile

void setup() {
  pinMode(4, INPUT_PULLUP); //SW1 pushbutton (encoder button)
  pinMode(15, INPUT_PULLUP); //SW2 pushbutton
  pinMode(A0, INPUT_PULLUP); //SW3 pushbutton
  pinMode(A1, INPUT_PULLUP); //SW4 pushbutton
  pinMode(A2, INPUT_PULLUP); //SW5 pushbutton
  pinMode(A3, INPUT_PULLUP); //SW6 pushbutton
  pinMode(14, INPUT_PULLUP); //SW7 pushbutton
  pinMode(16, INPUT_PULLUP); //SW8 pushbutton
  pinMode(10, INPUT_PULLUP); //SW9 pushbutton
  pinMode(8, INPUT_PULLUP); //SW10 pushbutton - acts as mode switch

  // Initialize serial, HID, and NeoPixel
  Serial.begin(9600);
  while (!Serial) { /* wait for serial to initialize */ }

  Serial.print("{\"status\":\"Free memory: ");
  Serial.print(freeMemory());
  Serial.println("\"}");

  Keyboard.begin();
  Mouse.begin();
  setupNeoPixels();

  // Load initial configuration from EEPROM. multiple profile support removed
  loadConfigFromEEPROM();

  Serial.println("{\"status\":\"System boot OK\"}");
}

void loop() {
  // Handle serial input
  handleSerialInput();

  // Multiple profile support, mode button switch profile
  // Ran out of RAM on a Leonardo, all multiple profile code removed

  // Check button presses for actions
  checkEncoderActions();

  // Check button presses for actions
  checkButtonActions();  
}

extern unsigned int __heap_start, *__brkval;
extern unsigned int __bss_end;
extern unsigned int __data_end;

int freeMemory() {
  int free_memory;
  
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  
  return free_memory;
}