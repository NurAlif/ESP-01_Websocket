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
#define ESP01_STATE_COMPLETE    7

struct PacketData {
    int state = 0;
    int conID;
    int readIndex = 0;
    int size;
    uint8_t header;
    char data[512];
};

class ESP01Serial{
    public:
        ESP01Serial(Stream *_serial);
        String dataJSON;

        //void sendCmd(char *data, int len);
        void sendCmd(String cmd, char findData[], int lenFindData, unsigned long timeout);
        void sendCmd(uint8_t *cmd, int lenCmd, char findData[], int lenFindData, unsigned long timeout);
        bool packetAvailable();
        void processPacket(PacketData *packetData, char data);
        PacketData popPacket();
        int packetCount = 0;
        void readSerial();
        void send(char *data, int len);
        void listenServer();
        int waitStartCmdResp();
        int waitStartCmdRespSynch();
        int dataStatus = 0;
        Stream *serial;
        PacketData packetBuffer[8];


    private:
        int bufferSize = 8;
        char data2wait[8];
        char inputBuffer[2048];
        int lastInputLength = 0;
        int data2waitlen;
        unsigned long timeoutDataWait;
        unsigned long startDataWait;

        char data[2048];

        void shiftPacketsToLeft();


};
#endif