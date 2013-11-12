#include "global.h"
#include "recvfile.h"

//main receive function
void receive(void *packet){
    uint8_t *recvMD5_Pointer = (uint8_t *) packet;
    
    
    unsigned int *fifthLine = (unsigned int *) packet;
    unsigned int a = ntohs(*(unsigned int *)(fifthLine + 4));
    int size = int((a >> 20) & 0xFFF);
    int seq = int((a << 8) & 0xFFF);

    //get MD5 version of the headers and payload
    uint8_t result[16];
    md5((uint8_t *) in, size + 4, result);
    
    //compare result with recvMD5
    if (!result == recvMD5) {
        return;
    }
    
    //compare sequence number
    if (stored.find(seq)) {
        return;
    }
    
    //temporarily store the received payload into a buffer
    stored.push(seq);
    
    //send back an acknowledge packet
    returnACK((unsigned int)seq);
    
    //write to file
    
}

//function that writes the received data to a local address
void outPut(string outPutAddress){
    
}

//send back an acknowledge packet, after successfully receiving the data
void returnACK(unsigned int acknowledge){
    //initialize a malloc
    uint8_t *toSend = (uint8_t *) malloc(20);
    
    //calculate an MD5 for the acknowledge number
    uint8_t result[16];
    md5((uint8_t *) acknowledge, 4, result);
    
    //add the MD5 into the malloc
    for (int i = 0; i < 16; i++) {
        *(uint8_t *)(tosend + i) = result[i];
    }
    
    //add the acknowledge number into the malloc
    *(uint8_t *)(toSend + 18) = (acknowledge <<16) & 0xff;
    *(uint8_t *)(toSend + 19) = (acknowledge <<24) & 0xff;
    
    //send the pointer
    send(toSend);
}

int getSizeByChar(void *packet){
    unsigned int *pck = (unsigned int *) packet;
    unsigned int n = ntohs(*(unsigned int *)(pck + 4));
    int size = int((a >> 20) & 0xFFF);
    return size;
}

int getSeq(void *packet){
    unsigned int *pck = (unsigned int *) packet;
    unsigned int n = ntohs(*(unsigned int *)(pck + 4));
    int seq = int((a << 8) & 0xFFF);
    return seq;
}
