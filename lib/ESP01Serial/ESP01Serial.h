#include <Arduino.h>

#ifndef ESP01Serial_h
#define ESP01Serial_h

#define ESP01_RESP_TIMEOUT  2
#define ESP01_RESP_FOUND    1
#define ESP01_RESP_NOTFOUND 0

#define ESP01_STATE_FIND_START   0
#define ESP01_STATE_FIND_CONID   1
#define ESP01_STATE_READ_CONID   2
#define ESP01_STATE_FIND_SIZE    3
#define ESP01_STATE_READ_SIZE    4
#define ESP01_STATE_FIND_DATA    5
#define ESP01_STATE_READ_DATA    6

struct PacketData {
    int state;
    int conID;
    int readIndex;
    int size;
    uint8_t header;
    String data;
} ;

class ESP01Serial{
    public:

        ESP01Serial(Stream *_serial);
        String dataJSON;

        //void sendCmd(char *data, int len);
        void sendCmd(String cmd, char findData[], int lenFindData, unsigned long timeout);
        void sendCmd(uint8_t *cmd, int lenCmd, char findData[], int lenFindData, unsigned long timeout);
        bool listenJSON(PacketData *packetData);
        void send(char *data, int len);
        void listenServer();
        int waitStartCmdResp();
        int waitStartCmdRespSynch();
        int dataStatus = 0;
        // lower abstraction


    private:
        Stream *serial;
        int bufferSize = 8;
        char data2wait[8];
        char inputBuffer[2048];
        int lastInputLength = 0;
        int data2waitlen;
        unsigned long timeoutDataWait;
        unsigned long startDataWait;

        char data[2048];
};
#endif