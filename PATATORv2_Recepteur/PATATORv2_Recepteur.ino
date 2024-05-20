#include <Wire.h>
#include <Stepper.h>

#define I2C_SLAVE_ADDRESS 8 // Adresse I2C du récepteur

const int stepsPerRevolution = 2048; // Nombre de pas par révolution pour le moteur 28BYJ-48
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // Initialisation du moteur pas à pas

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS); // Initialisation en tant que récepteur à l'adresse 8
  Wire.onReceive(receiveEvent); // Définir la fonction à appeler lors de la réception de données
  Serial.begin(9600);
  Serial.println("Receiver ready"); // Débogage : Récepteur initialisé

  myStepper.setSpeed(10); // Définir une vitesse par défaut (10 RPM)
}

void loop() {
  delay(100);
}

void receiveEvent(int bytes) {
  while (Wire.available()) {
    int received = Wire.read();
    Serial.println(received); // Débogage : Données reçues

    if (received == 1) {
      Serial.println("Command '1' received, rotating stepper 60 degrees."); // Débogage : Cas 1
      rotateStepper(60); // Tourner de 60 degrés
    } else if (received == 2) {
      Serial.println("Command '2' received, rotating stepper 120 degrees."); // Débogage : Cas 2
      rotateStepper(120); // Tourner de 120 degrés
    } else if (received == 3) {
      Serial.println("Command '3' received, rotating stepper 270 degrees."); // Débogage : Cas 3
      rotateStepper(270); // Tourner de 270 degrés
    } else {
      Serial.println("Unknown command received."); // Débogage : Cas inconnu
    }
  }
}

void rotateStepper(int degrees) {
  int steps = degrees * stepsPerRevolution / 360; // Conversion des degrés en pas
  myStepper.step(steps); // Tourner le moteur pas à pas
}
