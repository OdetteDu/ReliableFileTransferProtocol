
/* check whether the size of file is not larger than 1MB */
bool isSmallFile(char *path);

/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[], struct sockaddr_in *sin, char **filepath);

/* Send file to target when the size of file is not larger than 1MB */
void readFile_small(FILE* fp, map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length);

/* process the received acknowledgement for small file*/
void parseACK_small(char *recvACK, set<unsigned int> *ACK);

/* Send file to target when the size of file is larger than 1MB */
void sendFile_big();

/* construct the first packet that contains information of the big file,
 * including file length, segment length, MD5 of all segments */
void getInfoPacket(char *packet);

/* Construct the packet for big file, including segment number and sequence number */
void getPacket_big(char *packet, char *payload, unsigned int segNumber[], unsigned int segNumLen, unsigned short sequence);

/* process the received acknowledgement for big file */
void parseACK_big(char *recvACK);
