#include <Adafruit_NeoPixel.h>



#define PIN 5        // Pin où votre ruban LED est connecté
#define NUM_LEDS 30  // Nombre de LEDs sur votre ruban

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();  // Initialise toutes les LEDs à 'off'
}

void loop() {
  for (int value = 0; value <= 40; value++) {
    uint32_t color = getColorFromValue(value);
    setStripColor(color);
    delay(500);  // Attendre 0.5 seconde entre les changements de couleur
  }
}

uint32_t getColorFromValue(int value) {
  byte r, g, b;

  if (value <= 15) {
    // Interpolation entre bleu (0, 0, 255) et vert (0, 255, 0)
    r = 0;
    g = map(value, 0, 15, 0, 255);
    b = map(value, 0, 15, 255, 0);
  } else {
    // Interpolation entre vert (0, 255, 0) et rouge (255, 0, 0)
    r = map(value, 16, 30, 0, 255);
    g = map(value, 16, 30, 255, 0);
    b = 0;
  }

  return strip.Color(r, g, b);
}

void setStripColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
