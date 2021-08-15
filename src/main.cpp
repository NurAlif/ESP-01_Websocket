#include <Arduino.h>
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
  delay(500);

  if(ws.listenServer() > 0){
    PacketData packet = ws.popPacket();
    for(int i = 0; i < packet.size; i++)
      Serial.write(packet.data[i]);
    Serial.flush();

    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
}