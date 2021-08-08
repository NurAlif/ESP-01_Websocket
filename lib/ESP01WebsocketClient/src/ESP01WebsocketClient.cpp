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
    esp01->sendCmd("AT+CIPMUX=1", (char *)"OK", 2, 50000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    int findDoubleDots = HOST.indexOf(':');

    esp01->sendCmd("AT+CIPSTART=4,\"TCP\",\""+ (findDoubleDots > 0? HOST.substring(0, findDoubleDots): HOST) +"\","+ SERVER_PORT, (char *)"OK", 2, 5000000);
    int response = esp01->waitStartCmdRespSynch();
    if(response == ESP01_RESP_FOUND) Serial.println("pass0");
    if(response == ESP01_RESP_TIMEOUT) Serial.println("exit");
    esp01->sendCmd("AT+CIPSEND=4," +String(header.length()+4), (char *)">", 1, 5000000);
    response = esp01->waitStartCmdRespSynch();
    if(response == ESP01_RESP_FOUND) Serial.println("pass0");
    if(response == ESP01_RESP_TIMEOUT) Serial.println("exit");

    Serial.println("pass1");

    esp01->sendCmd(header, (char *)"OK", 2, (unsigned long)20000000);
    esp01->waitStartCmdRespSynch();
    esp01->sendCmd("AT+CIPCLOSE=0", (char *)"OK", 2, 1000000);
    esp01->waitStartCmdRespSynch();

    while(true){
        if(serial->available()){
            Serial.println(serial->readString());
        }
    }
}

void ESP01WebsocketClient::connectToWifi(String ssid, String password){
    esp01->sendCmd("AT", (char *)"OK", 2, 50000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    esp01->sendCmd("AT+CWMODE=1", (char *)"OK", 2, 50000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    esp01->sendCmd("AT+CWJAP=\""+ ssid +"\",\""+ password +"\"", (char *)"OK", 2, 20000000);
    if(esp01->waitStartCmdRespSynch() != ESP01_RESP_FOUND) return;

    // while(true){
    //     delay(1000);
    //     Serial.println("pass");
    // }
}