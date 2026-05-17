#include <Adafruit_NeoPixel.h>

#define LED_PIN   21   // Pin connected to NeoPixel data line
#define LED_COUNT 200   // Number of LEDs in your strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
//Blue, green, red

void setup() {
  strip.begin();        // Initialize NeoPixel strip
  strip.show();   
    // Set all LEDs to blue
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();       // Update LEDs
}      // Turn OFF all LEDs initially

void loop() {
  /*// Turn all LEDs red
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red color
  }
  strip.show();         // Update the strip
  delay(500);           // Wait 0.5 seconds

  // Turn all LEDs green
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green color
  }
  strip.show();         // Update the strip
  delay(500);           // Wait 0.5 seconds

  // Turn all LEDs blue
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue color
  }
  strip.show();         // Update the strip
  delay(500);           // Wait 0.5 seconds*/
}
