#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1       // Reset pin not used on 4-pin I2C modules
#define SCREEN_ADDRESS 0x3C    // Default I2C address for GME12864-11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed. Check wiring or I2C address."));
    for (;;); // Halt if display is not found
  }

  // Clear the buffer
  display.clearDisplay();

  // Draw a border rectangle
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  // Configure and print text
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 8);
  display.println(F("IT V Load = 25.5"));

  display.setTextSize(1);
  display.setCursor(16, 16);
  display.println(F("FC V Load = 25.5"));

  // Push buffer contents to the screen
  display.display();
}

void loop() {
  // Add dynamic updates here, followed by display.display()
}
