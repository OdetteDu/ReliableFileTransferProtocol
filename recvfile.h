
//main receive function
void receive(void *packet);

//function that writes the received data to a local address
void outPut(string outPutAddress);

//send back an acknowledge packet, after successfully receiving the data
void returnACK(unsigned int acknowledge);

//back up functions
int getSizeByChar(void *packet);
int getSeq(void *packet);

//variables
string address;
