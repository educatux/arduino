#include <Servo.h>

Servo myServo;  // Crée un objet Servo

void setup() {
  myServo.attach(A5);  // Attache le servo à la broche A5
}

void loop() {
  // Tourne à 180 degrés (à gauche)
  myServo.write(180);
  delay(1000);  // Attend 1 seconde

  // Tourne à 0 degré (à droite)
  myServo.write(0);
  delay(1000);  // Attend 1 seconde
}
