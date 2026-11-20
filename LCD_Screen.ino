#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
// (Change to 0x3F if 0x27 doesn't work, or 20, 4 for a 20x4 screen)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();          // Initialize the LCD
  lcd.backlight();     // Turn on the backlight

  // Print static text to the first row
  lcd.setCursor(0, 0); // (Column 0, Row 0)
  lcd.print("Hello, World!");

  // Print static text to the second row
  lcd.setCursor(0, 1); // (Column 0, Row 1)
  lcd.print("I2C LCD Ready");
}

void loop() {
  delay(2000);
  lcd.clear();
  // Print static text to the second row
  lcd.setCursor(0, 0); // (Column 0, Row 0)
  lcd.print("Vfc=5V");
  delay(2000);
  // Print static text to the second row
  lcd.setCursor(0, 1); // (Column 0, Row 1)
  lcd.print("Vit=4.4V");
}
