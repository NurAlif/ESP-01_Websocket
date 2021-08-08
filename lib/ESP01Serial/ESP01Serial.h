#include <Arduino.h>

#ifndef ESP01Serial_h
#define ESP01Serial_h

#define ESP01_RESP_TIMEOUT  2
#define ESP01_RESP_FOUND    1
#define ESP01_RESP_NOTFOUND 0

class ESP01Serial{
    public:

        ESP01Serial(Stream *_serial);

        //void sendCmd(char *data, int len);
        void sendCmd(String cmd, char findData[], int lenFindData, unsigned long timeout);
        void listen();
        void send(char *data, int len);
        void listenServer();
        int waitStartCmdResp();
        int waitStartCmdRespSynch();

        // lower abstraction


    private:
        Stream *serial;
        char data2wait[8];
        int data2waitlen;
        unsigned long timeoutDataWait;
        unsigned long startDataWait;

        char data[2048];
};
#endif