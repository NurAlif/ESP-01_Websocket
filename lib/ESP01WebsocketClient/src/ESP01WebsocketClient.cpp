#include "ESP01WebsocketClient.h"

ESP01WebsocketClient::ESP01WebsocketClient(String ip_address, String ws_port, Stream &serial){
    SERVER_IP = ip_address;
    SERVER_PORT = ws_port;

    serial = serial;
}

String ESP01WebsocketClient::setHeader(){
    header = 
        "GET " + URI + " HTTP/1.0\n" +
        "Host: " + HOST + "\n" +
        "Upgrade: websocket\n" +
        "Connection: Upgrade\n" +
        "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\n" +
        "Sec-WebSocket-Version: 13\n" +
        "Origin: IoT\n" +
        "\n";
}

void ESP01WebsocketClient::listenServer(){
    
}