#include <Arduino.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  Serial.println("asdasdasda");
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}