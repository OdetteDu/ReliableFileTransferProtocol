#include <queue>

//main receive function
void receive(void *packet);

//----------------helper-function--------------
void returnACK(unsigned int acknowledge);                                       //send back an acknowledge packet, after successfully receiving the data
bool packetCorrect(uint8_t * recvMD5_Pointer, uint8_t * result_Pointer);        //compare the received MD5 and the locally generated MD5
void send(char * toSend);                                                       //a dummy send function
bool haveStored(int sequence);

//----------------useless-functions------------
int getSizeByChar(void *packet);
int getSeq(void *packet);

//variables
std::queue<int> stored;
