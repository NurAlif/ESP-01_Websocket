#include "ESP01Serial.h"

ESP01Serial::ESP01Serial(Stream *_serial){
    serial = _serial;
}

ESP01Serial::ESP01Serial(Stream *_serial, int _pinReset){
    serial = _serial;
    pinReset = _pinReset;

    pinMode(pinReset, OUTPUT);
    digitalWrite(pinReset, HIGH);
}

void ESP01Serial::sendCmd(String cmd, char findData[], int lenFindData, unsigned long timeout){
    timeoutDataWait = timeout;
    data2waitlen = lenFindData;

    if(data2waitlen > DATA2WAIT_BUFFER_SIZE) data2waitlen = DATA2WAIT_BUFFER_SIZE;

    for(int i = 0; i < lenFindData; i++)
        data2wait[i] = findData[i];
    serial->println(cmd);
    
    lastInputLength = 0;

    startDataWait = micros();
}

void ESP01Serial::sendCmd(uint8_t *cmd, int lenCmd, char findData[], int lenFindData, unsigned long timeout){
    timeoutDataWait = timeout;
    data2waitlen = lenFindData;

    if(data2waitlen > DATA2WAIT_BUFFER_SIZE) data2waitlen = DATA2WAIT_BUFFER_SIZE;

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
        
        // Serial.print(currentChar);

        inputBuffer[lastInputLength++] = currentChar;
    }

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
            } 
        }
    }

    

    if(isSequenceFound)
        return ESP01_RESP_FOUND;
    return ESP01_RESP_NOTFOUND;
}

int ESP01Serial::waitStartCmdRespSync(){
    int response = 0;
    while(response == ESP01_RESP_NOTFOUND)
        response = waitStartCmdResp();

    // if(response == ESP01_RESP_FOUND) Serial.println("ESP01_RESP_FOUND");
    // else if(response == ESP01_RESP_TIMEOUT) Serial.println("ESP01_RESP_TIMEOUT");

    return response;
}

void ESP01Serial::resetHardware(){
    digitalWrite(pinReset, HIGH);
    delay(500);
    digitalWrite(pinReset, HIGH);
    delay(500);
}

void ESP01Serial::processPacket(PacketData *packetData, char currentChar){
    
    switch(packetData->state){
        case ESP01_STATE_FIND_START:
            if(currentChar == '+')
                packetData->state = ESP01_STATE_FIND_CONID;
            break;
        case ESP01_STATE_FIND_CONID:
            if(currentChar == ','){
                packetData->state = ESP01_STATE_READ_CONID;
                packetData->readIndex = 0;
            }
            break;
        case ESP01_STATE_READ_CONID:
            if(currentChar == ','){
                packetData->state = ESP01_STATE_READ_SIZE;
                packetData->readIndex = 0;
            }
            else{
                packetData->conID = 
                (packetData->readIndex <= 0? 0 : packetData->conID*10) 
                + (int)(currentChar - '0');
                packetData->readIndex++;
            }
            break;
        case ESP01_STATE_READ_SIZE:
            if(currentChar == ':'){
                packetData->state = ESP01_STATE_READ_DATA;
                packetData->readIndex = 0;
            }
            else{
                packetData->size = 
                (packetData->readIndex <= 0? 0 : packetData->size*10) 
                + (int)(currentChar - '0');
                packetData->readIndex++;
            }
            break;
        case ESP01_STATE_READ_DATA:
            packetData->data[packetData->readIndex] = currentChar;
            if(packetData->readIndex <= 0) packetData->header = uint8_t(currentChar);
            packetData->readIndex++;
            if(packetData->readIndex >= packetData->size) {
                packetData->state = ESP01_STATE_COMPLETE;
                delay(1);
            }
            break;
        default:
            if(currentChar == '+')
                packetData->state = ESP01_STATE_FIND_CONID;
    }
}

void ESP01Serial::readSerial(){
    PacketData *packetCurrent = &packetBuffer[packetCount];
    while(serial->available()){
        char currentChar = serial->read();
        // Serial.print(currentChar);
        
        processPacket(packetCurrent, currentChar);

        if(packetCurrent->state == ESP01_STATE_COMPLETE){
            packetCount++;
            if(packetCount >= PACKETS_BUFFER_SIZE) shiftPacketsToLeft();
            packetCurrent = &packetBuffer[packetCount];
        }
    }
}

PacketData ESP01Serial::popPacket(){
    PacketData packetData = packetBuffer[0];
    shiftPacketsToLeft();
    return packetData;
}

PacketData ESP01Serial::peekPacket(){
    PacketData packetData = packetBuffer[0];
    return packetData;
}

bool ESP01Serial::packetAvailable(){
    return packetCount > 0? true : false;
}

void ESP01Serial::shiftPacketsToLeft(){
    for(int i = 1; i < packetCount; i++){
        packetBuffer[i-1] = packetBuffer[i];
    }
    packetBuffer[--packetCount].state = ESP01_STATE_FIND_START;
}
