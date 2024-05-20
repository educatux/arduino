#include <Servo.h>

Servo myServo;  // Création d'un objet servo pour contrôler le servomoteur

int pos = 0;    // Variable pour stocker la position du servo

void setup() {
  myServo.attach(15);  // Attache le servomoteur au pin 9 de l'Arduino
}

void loop() {
  // Déplacer le servomoteur de 0 à 180 degrés
  for (pos = 0; pos <= 180; pos += 1) {
    myServo.write(pos);  // Définir la position du servomoteur
    delay(20);           // Pause pour ralentir la vitesse de mouvement
  }

  delay(1000);  // Pause d'une seconde

  // Déplacer le servomoteur de 180 à 0 degrés
  for (pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);  // Définir la position du servomoteur
    delay(20);           // Pause pour ralentir la vitesse de mouvement
  }

  delay(1000);  // Pause d'une seconde
}
