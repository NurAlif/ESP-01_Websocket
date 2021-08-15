#include <Arduino.h>


#ifndef ESP01Serial_h
#define ESP01Serial_h

#define SERIAL_BUFFER_SIZE 256

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

#define DATA2WAIT_BUFFER_SIZE 8
#define PACKETS_BUFFER_SIZE 8
#define DATA_INPUT_BUFFER_SIZE 2048

struct PacketData {
    int state = 0;
    int conID = -1;
    int readIndex = 0;
    int size;
    uint8_t header;
    char data[512];
};

class ESP01Serial{
    public:
        Stream *serial;

        ESP01Serial(Stream *_serial);
        ESP01Serial(Stream *_serial, int pinReset);

        void sendCmd(String cmd, char findData[], int lenFindData, unsigned long timeout);
        void sendCmd(uint8_t *cmd, int lenCmd, char findData[], int lenFindData, unsigned long timeout);
        bool packetAvailable();
        PacketData popPacket();
        PacketData peekPacket();
        void readSerial();
        int waitStartCmdResp();
        int waitStartCmdRespSync();
        void resetHardware();


    private:
        char data2wait[DATA2WAIT_BUFFER_SIZE];
        char inputBuffer[DATA_INPUT_BUFFER_SIZE];
        int lastInputLength = 0;
        int data2waitlen;
        unsigned long timeoutDataWait;
        unsigned long startDataWait;
        int pinReset;
        int packetCount = 0;

        char data[DATA_INPUT_BUFFER_SIZE];
        PacketData packetBuffer[PACKETS_BUFFER_SIZE];

        void shiftPacketsToLeft();
        void processPacket(PacketData *packetData, char data);


};
#endif