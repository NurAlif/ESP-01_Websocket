#include <Arduino.h>
#include "ESP01Serial.h"
#include "ESP01WebsocketClient.h"

ESP01WebsocketClient ws("147.139.175.105:8177", "8177", "/", &Serial1);

void setup() {  
  delay(5000);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(1000000);
  Serial1.begin(115200);
  // ws.connectToWifi("Kerajinan kayu", "HKI18A7Nag11");
  while(!ws.upgradeToWS()){delay(WS_RECONNECT_INTERVAL);}
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  Serial.println("-");
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  ws.listenServer();
}
/*
-13-10-43-73-80-68-44-52-44-50-58-137-0-
-13-10-43-73-80-68-44-52-44-50-58-137-0-
-13-10-43-73-80-68-44-52-44-49-52-58-129-12-123-34-99-109-100-34-58-34-79-75-34-125

*/