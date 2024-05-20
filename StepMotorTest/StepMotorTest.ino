#include <Stepper.h>

const int stepsPerRevolution = 2048; // Nombre de pas par révolution pour le moteur 28BYJ-48
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // Initialisation du moteur pas à pas

void setup() {
  Serial.begin(9600); // Initialisation de la communication série
  Serial.println("Stepper test starting...");
}

void loop() {
  // Tourner dans le sens horaire
  Serial.println("Clockwise");
  myStepper.setSpeed(16); // Définir la vitesse du moteur (10 RPM)
  myStepper.step(1024); // Effectuer une révolution complète

  delay(1000); // Attendre 1 seconde

  // Tourner dans le sens antihoraire
  Serial.println("Counterclockwise");
  myStepper.setSpeed(16); // Définir la vitesse du moteur (10 RPM)
  myStepper.step(-stepsPerRevolution); // Effectuer une révolution complète en sens inverse

  delay(1000); // Attendre 1 seconde
}
