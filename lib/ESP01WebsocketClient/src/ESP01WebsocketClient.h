#include <Arduino.h>
#include "ESP01Serial.h"

#ifndef ESP01WebsocketClient_h
#define ESP01WebsocketClient_h

#define ESP01WS_OP_PONG 10
#define ESP01WS_OP_PING 9
#define ESP01WS_OP_TEXT 1
#define ESP01WS_STATE_CONNECTED 2
#define ESP01WS_STATE_CONNECTING 1
#define ESP01WS_STATE_DISCONNECTED 0

#define TIMEOUT_SERVER_PING 30000 // 1.0min 
#define WS_RECONNECT_INTERVAL 10000 // 0.5min 

class ESP01WebsocketClient{
    public:
        ESP01WebsocketClient(String host, String ws_port, String uri, ESP01Serial *_esp01);
        ESP01WebsocketClient(String host, String ws_port, String uri, Stream *_serial);

        void connectToWifi(String ssid, String password);
        bool upgradeToWS();
        int listenServer();
        void sendPacket(int opcode, int payloadLen, String payload);
        void sendPacket(int opcode);

        PacketData popPacket();
        

    private:
        Stream *serial;
        String SERVER_URI = "/";
        String SERVER_PORT = "8177";

        String HOST = "";
        String header = "";

        ESP01Serial *esp01;

        unsigned long timerServerPing = 0;
        unsigned long timerServerReconnect = 0;
};




#endif /* ESP01WebsocketClient_h */