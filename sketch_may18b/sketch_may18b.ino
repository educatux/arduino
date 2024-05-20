#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

// Définir les broches de connexion
#define TFT_CS     10
#define TFT_RST    8
#define TFT_DC     9

// Initialiser l'écran avec les broches définies
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialiser la communication série pour le débogage
  Serial.begin(9600);
  Serial.println("Testing ILI9341!");

  // Initialiser l'écran
  tft.begin();

  // Remplir l'écran avec une couleur (bleu)
  tft.fillScreen(ILI9341_BLUE);
}

void loop() {
  // Rien à faire ici
}
