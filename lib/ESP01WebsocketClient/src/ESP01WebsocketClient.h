#include <Arduino.h>
#include "ESP01Serial.h"

#ifndef ESP01WebsocketClient_h
#define ESP01WebsocketClient_h

class ESP01WebsocketClient{
    public:

        ESP01WebsocketClient(String ip_address, String ws_port, Stream &serial);

        void connectToWifi();
        void upgradeToWS();
        void send(char *data, int len);
        void listenServer();

        // lower abstraction
        

    private:
        Stream *serial;
        String URL = "";
        String HOST = "";
        String URI = "";

        String SERVER_IP = "147.139.175.105";
        String SERVER_PORT = "8177";

        String header = "";

        String setHeader();
        ESP01Serial *esp01;
};




#endif /* ESP01WebsocketClient_h */