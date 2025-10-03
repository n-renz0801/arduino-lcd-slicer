/*
  Pot-Controlled LCD Message Slicer
  ---------------------------------
  Hardware:
    - Arduino Nano
    - 20x4 I2C LCD at address 0x27
    - Potentiometer wiper -> A0, ends -> 5V and GND

  Behavior:
    - Reads the potentiometer on A0.
    - Maps the analog reading to a character count from the message.
    - Displays that many leading characters on the LCD (row 0, col 0).
    - Prints debug info to Serial: analog value and the exact text shown.
    - **Note:** This sketch only prints on a single row (row 0). 
    If the message exceeds the LCD’s width (20 chars), it is capped 
    to fit the row.

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>   // For strlen, strncpy

// ----------------------- Configuration -----------------------
static const uint8_t  LCD_ADDR   = 0x27;   // I2C address for most 20x4 displays
static const uint8_t  LCD_COLS   = 20;
static const uint8_t  LCD_ROWS   = 4;

static const uint8_t  POT_PIN    = A0;     // Analog pin for potentiometer
static const long     BAUD_RATE  = 9600;   // Serial baud rate
static const uint16_t LOOP_DELAY = 100;    // Loop delay in ms (matches original)

// Message to be sliced and displayed
static const char MESSAGE[] = "lcd pot";

// Analog range (Arduino UNO/MEGA/etc. use 10-bit ADC: 0..1023)
static const int ANALOG_MIN = 0;
static const int ANALOG_MAX = 1023;        // Use the full ADC range

// ----------------------- Globals -----------------------------
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// A small buffer to hold the currently displayed slice.
// Size it to the LCD width + 1 for the null terminator.
char displayBuffer[LCD_COLS + 1];

// ----------------------- Helpers -----------------------------

/**
 * computeSliceLen: Map the analog reading to a number of characters
 * to display from MESSAGE (0..strlen(MESSAGE)).
 */
int computeSliceLen(int analogValue) {
  // Map the analog value to [0, messageLen]
  const int messageLen = (int)strlen(MESSAGE);

  // Arduino's map may return values slightly outside range if inputs go out-of-bounds,
  // so we constrain to be safe.
  long mapped = map(analogValue, ANALOG_MIN, ANALOG_MAX, 0, messageLen);
  mapped = constrain(mapped, 0, messageLen);

  return (int)mapped;
}

/**
 * buildSlice: Copy the first 'count' characters from MESSAGE into displayBuffer.
 * Ensures displayBuffer is null-terminated.
 */
void buildSlice(int count) {
  // Limit to both the requested count and the LCD width
  int capped = count;
  if (capped > LCD_COLS) capped = LCD_COLS;

  // Copy and null-terminate
  strncpy(displayBuffer, MESSAGE, capped);
  displayBuffer[capped] = '\0';
}

/**
 * showOnLcd: Clear the LCD and print the current buffer on the first row.
 */
void showOnLcd() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(displayBuffer);
}

/**
 * debugPrint: Print analog reading and the actual displayed text to Serial.
 */
void debugPrint(int analogValue) {
  Serial.print("Analog Reading: ");
  Serial.print(analogValue);
  Serial.print(" | Display: \"");
  Serial.print(displayBuffer);
  Serial.println("\"");
}


void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(BAUD_RATE);
  // Optional: small startup message
  Serial.println("Pot-Controlled LCD Message Slicer - Starting...");
}

void loop() {
  // 1) Read the raw analog value from the potentiometer
  int analogValue = analogRead(POT_PIN);      

  // 2) Compute how many characters to show
  int sliceLen = computeSliceLen(analogValue);

  // 3) Build the display text
  buildSlice(sliceLen);

  // 4) Update LCD
  showOnLcd();

  // 5) Debug to Serial
  debugPrint(analogValue);

  // 6) Pace the loop
  delay(LOOP_DELAY);
}