#include <Arduino.h>
#include "ESP01Serial.h"
#include "ESP01WebsocketClient.h"

ESP01Serial esp01(&Serial1);
ESP01WebsocketClient ws("147.139.175.105:8177", "8177", "/", &esp01);

void setup() {  
  delay(5000);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  ws.connectToWifi("Kerajinan kayu", "HKI18A7Nag11");
  ws.upgradeToWS();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  Serial.println("asdasdasda");
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}