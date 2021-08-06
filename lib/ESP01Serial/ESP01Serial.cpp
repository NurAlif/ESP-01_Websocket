#include "ESP01Serial.h"

ESP01Serial::ESP01Serial(Stream *_serial){
    serial = _serial;
}

bool ESP01Serial::sendCmd(String cmd, char findData[], int lenFindData, long timeout){
    timeoutDataWait = timeout;
    data2waitlen = lenFindData;

    if(data2waitlen > 8) data2waitlen = 8;

    for(int i = 0; i < lenFindData; i++)
        data2wait[i] = findData[i];
    
    serial->println(cmd);
    startDataWait = micros();
}

int ESP01Serial::waitStartCmdResp(){
    
    if(micros() - startDataWait > timeoutDataWait) return ESP01_RESP_TIMEOUT;

    if(serial->find(data2wait, data2waitlen))
        return ESP01_RESP_FOUND;
    return ESP01_RESP_NOTFOUND;
}

void ESP01Serial::listen(){
    
}
