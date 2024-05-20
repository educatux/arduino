#include <Stepper.h>

const int stepsPerRevolution = 2048; // Nombre de pas pour une révolution complète (pour un moteur 28BYJ-48)

// Initialisation de l'objet Stepper avec les broches utilisées
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Régler la vitesse du moteur à 16 RPM
  myStepper.setSpeed(16);
}

void loop() {
  // Mouvement de 60 degrés
  myStepper.step(stepsPerRevolution * 60 / 360);
  delay(10000); // Pause de 10 secondes

  // Mouvement de 120 degrés
  myStepper.step(stepsPerRevolution * 120 / 360);
  delay(10000); // Pause de 10 secondes

  // Mouvement de 270 degrés
  myStepper.step(stepsPerRevolution * 270 / 360);
  delay(10000); // Pause de 10 secondes

  // Retour à la position initiale (60 + 120 + 270 = 450 -> 450 - 360 = 90)
  myStepper.step(-stepsPerRevolution * 90 / 360);
  delay(10000); // Pause de 10 secondes
}
