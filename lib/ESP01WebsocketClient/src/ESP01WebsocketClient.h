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

#define TIMEOUT_SERVER_PING 60000 // 1.5min 
#define WS_RECONNECT_INTERVAL 30000 // 0.5min 

class ESP01WebsocketClient{
    public:

        int connected = false;
        ESP01WebsocketClient(String host, String ws_port, String uri, ESP01Serial *_esp01);
        ESP01WebsocketClient(String host, String ws_port, String uri, Stream *_serial);

        void connectToWifi(String ssid, String password);
        bool upgradeToWS();
        void send(char *data, int len);
        void listenServer();
        void listenPing();
        void sendPacket(int opcode, int payloadLen, String payload);
        void sendPacket(int opcode);
        // lower abstraction
        

    private:
        Stream *serial;
        String SERVER_URI = "/";
        String SERVER_PORT = "8177";

        String HOST = "";
        String header = "";

        String setHeader();
        ESP01Serial *esp01;

        unsigned long timerServerPing = 0;
        unsigned long timerServerReconnect = 0;

        PacketData packetData;
};




#endif /* ESP01WebsocketClient_h */