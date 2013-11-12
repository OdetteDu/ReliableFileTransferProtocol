#include "global.h"
#include "recvfile.h"
#include <queue>

//--------------------------------------------------main receive function------------------------------------
void receive(void *packet){
    
    //******* needing codes to get the output address and file name from the inputted commands *******
    
    
    uint8_t *recvMD5_Pointer = (uint8_t *) packet;                //Create a pointer pointing to the start of the data packet to get the MD5 information
    unsigned int *fifthLine = (unsigned int *) packet;            //Create a pointer pointing to the fifth line of the data packet
    fifthLine += 4;
    int size = int((*fifthLine >> 20) & 0xFFF);                    //Getting the size of the payload from the fifth line of the packet
    if (size > 2048) {                                            //Since the data packet sent will never exceed 2048 byte, there must be an error occurred if size is greater than 2048
        return;
    }
    int seq = int((*fifthLine << 8) & 0xFFF);                      //Getting the sequence number from the fifth line of the packet

    uint8_t result[16];                                           //get MD5 version of the headers and payload
    md5((uint8_t *) recvMD5_Pointer, size + 4, result);
    uint8_t * result_Pointer = (uint8_t *) result;                           //create a pointer pointing to this MD5
    
    //compare result with recvMD5
    if (packetCorrect(recvMD5_Pointer, result_Pointer)) {
        return;
    }
    
    //compare sequence number
    if (haveStored(seq)) {
        return;
    }
    
    //temporarily store the received payload into a buffer
    stored.push(seq);
    
    //******* needing a function to store the payload to a buffer ********
    
    //send back an acknowledge packet
    returnACK((unsigned int)seq);
    
    //write to file
    //******* needing a function to output to the desired address ********
    
    //end
    return;
    
}

//----------------------------------------------helper-functions---------------------------------------------

//send back an acknowledge packet, after successfully receiving the data
void returnACK(unsigned int acknowledge){
    //initialize a malloc
    char *toSend = (char *) malloc(20);
    
    //calculate an MD5 for the acknowledge number
    uint8_t result[16];
    md5((uint8_t *) (toSend + 16), 4, result);
    
    //add the MD5 into the malloc
    for (int i = 0; i < 16; i++) {
        *(char *)(toSend + i) = htons(result[i]);
    }
    
    //add the acknowledge number into the malloc
    *(char *)(toSend + 18) = htons((acknowledge <<16) & 0xff);
    *(char *)(toSend + 19) = htons((acknowledge <<24) & 0xff);
    
    //send the pointer
    send(toSend);
}

//haveStored
bool haveStored(int sequence){
    std::queue<int> tempQ = stored;
    while (!tempQ.empty()) {
        if (tempQ.front() != sequence) {
            tempQ.pop();
        }
        else{
            return true;
        }
    }
    return false;
}

//compare the received MD5 and the locally generated MD5
bool packetCorrect(uint8_t * recvMD5_Pointer, uint8_t * result_Pointer){
    for (int i = 0; i < 16; i++) {
        if(* (recvMD5_Pointer + i) != * (result_Pointer + i))
            return false;
    }
    return true;
    
}

//dummy send function
void send(char * toSend){
    return;
}


//----------------------------------------------not-quite-useful-functions----------------------------------------------------
int getSizeByChar(void *packet){
    unsigned int *pck = (unsigned int *) packet;
    unsigned int n = ntohs(*(unsigned int *)(pck + 4));
    int size = int((n >> 20) & 0xFFF);
    return size;
}

int getSeq(void *packet){
    unsigned int *pck = (unsigned int *) packet;
    unsigned int n = ntohs(*(unsigned int *)(pck + 4));
    int seq = int((n << 8) & 0xFFF);
    return seq;
}
