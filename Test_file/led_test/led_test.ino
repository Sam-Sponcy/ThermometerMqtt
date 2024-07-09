#include <Adafruit_NeoPixel.h>

#define PIN           6  // Broche de données du ruban LED
#define PIN2           5  // Broche de données du ruban LED
#define NUM_LEDS     30  // Nombre total de LEDs dans le ruban
#define BRIGHTNESS  100  // Luminosité (0-255)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();           // Initialisation du ruban LED
  strip2.begin();           // Initialisation du ruban LED
  strip.setBrightness(BRIGHTNESS); // Réglage de la luminosité
  strip2.setBrightness(BRIGHTNESS); // Réglage de la luminosité
  strip.show();            // Mise à jour des LEDs
  strip2.show();            // Mise à jour des LEDs
}

void loop() {

  Serial.println("Led allumer");
  // Allumer toutes les LEDs en rouge
  colorWipe(strip2.Color(255, 255, 0), 50); // Rouge
  delay(1000); // Attendre 1 seconde

  // Allumer toutes les LEDs en vert
  colorWipe(strip2.Color(0, 255, 0), 50); // Vert
  delay(1000); // Attendre 1 seconde

  // Allumer toutes les LEDs en bleu
  colorWipe(strip2.Color(255, 0, 255), 50); // Bleu
  delay(1000); // Attendre 1 seconde
}

// Remplissage de la couleur de toute la bande
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip2.numPixels(); i++) {
    strip2.setPixelColor(i, color); // Définir la couleur du pixel
    strip2.show(); // Mettre à jour le ruban LED avec la nouvelle couleur
    delay(wait); // Attendre un court laps de temps pour une animation fluide
  }
}
