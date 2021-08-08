#include <Arduino.h>
#include "ESP01Serial.h"

#ifndef ESP01WebsocketClient_h
#define ESP01WebsocketClient_h

class ESP01WebsocketClient{
    public:
        ESP01WebsocketClient(String host, String ws_port, String uri, ESP01Serial *_esp01);

        void connectToWifi(String ssid, String password);
        void upgradeToWS();
        void send(char *data, int len);
        void listenServer();

        // lower abstraction
        

    private:
        Stream *serial;
        String SERVER_URI = "/";
        String SERVER_PORT = "8177";

        String HOST = "";
        String header = "";

        String setHeader();
        ESP01Serial *esp01;
};




#endif /* ESP01WebsocketClient_h */