#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ST7735.h> // Include Adafruit_ST7735 library to control the display
#include <Fonts/FreeMono9pt7b.h>

// Pin configuration for the Arduino Uno
  #define TFT_CS        10
  #define TFT_RST        7 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8

const int pwmPin = 9; // PWM Pin (muss ein Timer-fähiger Pin sein)
const int analogPin = A0; // Analoger Eingangspin
int analogValue = 0; // Variable zum Speichern des analogen Werts
int pwmValue = 0; // PWM-Wert

int lastAnalogValue = -1; // Initialize with an impossible value


// Initialize Adafruit ST7735S library
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  
  // Initialize display
  tft.initR(INITR_BLACKTAB);      // Initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);   // Fill screen with black color
  tft.setRotation(1);

  // Set text color, text size, and then print a message
  tft.setTextColor(ST7735_WHITE);  
  tft.setFont(&FreeMono9pt7b);
  tft.setTextWrap(true);
  tft.setCursor(0, 46);
  tft.println("github.com");
  tft.println("/Johann-Schmid");
  tft.println("/uvLed");
  delay(10000);

  pinMode(pwmPin, OUTPUT); // Setze den PWM-Pin als Ausgang
  // Konfiguriere Timer 1 für eine Frequenz von 10 kHz
  TCCR1A = _BV(COM1A1) | _BV(WGM11); // Setze den Modus für Fast PWM
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // Kein Prescaling
  ICR1 = 1599; // Setze den Top-Wert für 10 kHz
}

void loop() {
    analogValue = analogRead(analogPin); // Read the current analog value

  // Check if the analog value has changed
  if (abs(analogValue - lastAnalogValue) > 5) {
    // Map the analog value to the PWM range and apply it
    pwmValue = map(analogValue, 0, 1023, 0, ICR1);
    OCR1A = pwmValue;

    // Clear the screen and draw the rectangle with the new width
    tft.fillScreen(ST7735_BLACK);
    tft.fillRoundRect(0, 0, map(analogValue, 0, 1023, 0, 160), 50, 5, ST77XX_GREEN);

    // Display the new analog value
    tft.setCursor(0, 100);
    tft.print("Analog Value: ");
    tft.print(map(analogValue, 0, 1023, 0, 100));
    tft.print("%");

    // Update the last analog value
    lastAnalogValue = analogValue;
  }
  delay(100);
}
