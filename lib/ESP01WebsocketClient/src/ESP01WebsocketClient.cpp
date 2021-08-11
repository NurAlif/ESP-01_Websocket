#include "ESP01WebsocketClient.h"

ESP01WebsocketClient::ESP01WebsocketClient(String host, String ws_port, String uri, ESP01Serial *_esp01){
    SERVER_PORT = ws_port;
    SERVER_URI = uri;
    esp01 = _esp01;

    HOST = host;

    header = "GET ws://" + HOST + SERVER_URI + " HTTP/1.1\n" +
        "Host: " + HOST + "\n" +
        "Upgrade: websocket\n" +
        "Connection: Upgrade\n" +
        "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\n" +
        "Sec-WebSocket-Version: 13\n" +
        "Origin: IoT\n" +
        "\n";
}

void ESP01WebsocketClient::upgradeToWS(){

    esp01->sendCmd("AT+CIPCLOSE=0", (char *)"OK", 2, 1000000);
    esp01->waitStartCmdRespSynch();
    esp01->sendCmd("AT+CIPMUX=1", (char *)"OK", 2, 100000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    int findDoubleDots = HOST.indexOf(':');

    esp01->sendCmd("AT+CIPSTART=4,\"TCP\",\""+ (findDoubleDots > 0? HOST.substring(0, findDoubleDots): HOST) +"\","+ SERVER_PORT, (char *)"OK", 2, 5000000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;
    esp01->sendCmd("AT+CIPSEND=4," +String(header.length()+4), (char *)">", 1, 5000000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    esp01->sendCmd(header, (char *)"OK", 2, 1000000);
    esp01->waitStartCmdRespSynch();
    esp01->sendCmd("AT+CIPCLOSE=0", (char *)"OK", 2, 1000000);
    esp01->waitStartCmdRespSynch();
}

void ESP01WebsocketClient::connectToWifi(String ssid, String password){
    esp01->sendCmd("AT", (char *)"OK", 2, 50000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    esp01->sendCmd("AT+CWMODE=1", (char *)"OK", 2, 50000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    esp01->sendCmd("AT+CWJAP=\""+ ssid +"\",\""+ password +"\"", (char *)"OK", 2, 20000000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;
}

void ESP01WebsocketClient::listenServer(){
    
    sendPacket(ESP01WS_OP_TEXT, 11, "HELLO WORLD");
    if(esp01->listenJSON(&packetData)){
        if(packetData.header == 137){
            Serial.print("PING !!!");

            sendPacket(ESP01WS_OP_PONG);
        }
        Serial.print("DATA IN >");
        Serial.println(packetData.data);
    }
}

void ESP01WebsocketClient::sendPacket(int opcode, int payloadLen, String payload){
    uint8_t header[4];
    header[0] = 128;
    header[0] += opcode;

    //length 
    header[1] = payloadLen;
    int lenHeader = 2;
    if(payloadLen > 125){
        header[1] = 126;
        header[3] = payloadLen < 256? 0 : payloadLen - 256;
        header[2] = payloadLen < 256? 0 : payloadLen >> 8;
    }

    uint8_t packet[1024];
    int lenTotal = lenHeader + payloadLen;

    int i = 0;
    for(; i < lenHeader; i++)
        packet[i] = header[i];
    for(; i < payloadLen; i++){
        packet[i] = payload[i-lenHeader];
    }

    // send
    esp01->sendCmd("AT+CIPSEND=4," +String(lenTotal), (char *)">", 1, 50000);
    esp01->waitStartCmdRespSynch();
    esp01->sendCmd((uint8_t *)packet, lenTotal, (char *)"OK", 2, 50000);
}
void ESP01WebsocketClient::sendPacket(int opcode){
    uint8_t header[2];
    header[0] = 128;
    header[0] += opcode;

    //length 
    header[1] = 0;
    esp01->sendCmd("AT+CIPSEND=4," +String(2), (char *)">", 1, 50000);
    esp01->waitStartCmdRespSynch();
    esp01->sendCmd((uint8_t *)header, 2, (char *)"OK", 2, 50000);
}