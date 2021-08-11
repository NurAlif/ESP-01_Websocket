#include "ESP01Serial.h"

ESP01Serial::ESP01Serial(Stream *_serial){
    serial = _serial;
    #define SERIAL_BUFFER_SIZE 256
}

void ESP01Serial::sendCmd(String cmd, char findData[], int lenFindData, unsigned long timeout){
    timeoutDataWait = timeout;
    data2waitlen = lenFindData;

    if(data2waitlen > 8) data2waitlen = 8;

    for(int i = 0; i < lenFindData; i++)
        data2wait[i] = findData[i];
    Serial.println(cmd);
    serial->println(cmd);
    
    lastInputLength = 0;

    startDataWait = micros();
}

void ESP01Serial::sendCmd(uint8_t *cmd, int lenCmd, char findData[], int lenFindData, unsigned long timeout){
    timeoutDataWait = timeout;
    data2waitlen = lenFindData;

    if(data2waitlen > 8) data2waitlen = 8;

    for(int i = 0; i < lenFindData; i++)
        data2wait[i] = findData[i];
    
    for(int i = 0; i < lenCmd; i++){
        serial->write(cmd[i]);
    }
    serial->flush();
    
    lastInputLength = 0;

    startDataWait = micros();
}

int ESP01Serial::waitStartCmdResp(){
    
    if(micros() - startDataWait > timeoutDataWait) return ESP01_RESP_TIMEOUT;

    int searchIndex = 0;

    bool isSequenceFound = false;

    int newLen = 0;

    while(serial->available()){
        newLen++;
        char currentChar = serial->read();
        
        Serial.print(currentChar);

        inputBuffer[lastInputLength++] = currentChar;
    }

    if(lastInputLength > 2048) Serial.println("OVERFLOW");

    if(newLen > 0)
    for(int i = 0; i < lastInputLength; i++){
        char currentChar = inputBuffer[i];
        // Serial.print(currentChar);
        if(!isSequenceFound){
            if(searchIndex <= 0 && currentChar == data2wait[0]){
                searchIndex = 1;
            }
            else if(searchIndex > 0 && currentChar == data2wait[searchIndex]){
                searchIndex++;
            }else{
                searchIndex = 0;
            }
            if(searchIndex >= data2waitlen){
                isSequenceFound = true;
                Serial.println("FOUND");
            } 
        }
    }

    

    if(isSequenceFound)
        return ESP01_RESP_FOUND;
    return ESP01_RESP_NOTFOUND;
}

int ESP01Serial::waitStartCmdRespSynch(){
    int response = 0;
    while(response == ESP01_RESP_NOTFOUND)
        response = waitStartCmdResp();

    if(response == ESP01_RESP_FOUND) Serial.println("ESP01_RESP_FOUND");
    else if(response == ESP01_RESP_TIMEOUT) Serial.println("ESP01_RESP_TIMEOUT");

    return response;
}

bool ESP01Serial::listenJSON(PacketData *PacketData){
    while(serial->available()){

        char currentChar = serial->read();
        
        switch(PacketData->state){
            case ESP01_STATE_FIND_START:
                if(currentChar == '+')
                    PacketData->state = ESP01_STATE_FIND_CONID;
                break;
            case ESP01_STATE_FIND_CONID:
                if(currentChar == ','){
                    PacketData->state = ESP01_STATE_READ_CONID;
                    PacketData->readIndex = 0;
                }
                break;
            case ESP01_STATE_READ_CONID:
                if(currentChar == ','){
                    PacketData->state = ESP01_STATE_READ_SIZE;
                    PacketData->readIndex = 0;
                }
                else{
                    PacketData->conID = 
                    (PacketData->readIndex <= 0? 0 : PacketData->conID*10) 
                    + (int)(currentChar - '0');
                    PacketData->readIndex++;
                }
                break;
            case ESP01_STATE_READ_SIZE:
                if(currentChar == ':'){
                    PacketData->state = ESP01_STATE_READ_DATA;
                    PacketData->readIndex = 0;
                    PacketData->data = "";
                }
                else{
                    PacketData->size = 
                    (PacketData->readIndex <= 0? 0 : PacketData->size*10) 
                    + (int)(currentChar - '0');
                    PacketData->readIndex++;
                }
                break;
            case ESP01_STATE_READ_DATA:
                PacketData->data += currentChar;
                if(PacketData->readIndex <= 0) PacketData->header = uint8_t(currentChar);
                PacketData->readIndex++;
                if(PacketData->readIndex >= PacketData->size) {
                    PacketData->state = ESP01_STATE_FIND_START;
                    return true;
                }
                break;
            default:
                if(currentChar == '+')
                    PacketData->state = ESP01_STATE_FIND_CONID;
        }
    }
    return false;
}
