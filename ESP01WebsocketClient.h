#ifndef ESP01WebsocketClient_h
#define ESP01WebsocketClient_h


#ifdef __cplusplus
 extern "C" {
#endif

class ESP01WebsocketClient{
  public:
    String SERVER_IP = "147.139.175.105";
    String SERVER_PORT = "8177";

    String URL = "";
    String HOST = "";
    String URI = "";

    String header = 
        "GET " + URI + " HTTP/1.0\n" +
        "Host: " + HOST + "\n" +
        "Upgrade: websocket\n" +
        "Connection: Upgrade\n" +
        "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\n" +
        "Sec-WebSocket-Version: 13\n" +
        "Origin: IoT\n" +
        "\n";

    ESP01WebsocketClient(String, String);

    void connectToWifi();
    void upgradeToWS();
    void send(char *data, int len);
    void listenServer();
}


#ifdef __cplusplus
}
#endif



#endif /* DXL_H_ */