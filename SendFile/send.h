
#define SMALL_SIZE 1048576

/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[]);

/* Send file to target when the size of file is not larger than 1MB */
void sendFile_small();

/* Construct the small packet, including MD5, length of payload, offset in the
 * whole file and sequence number */
void getPacket_small(char *packet, char *payload, unsigned short payloadLen, unsigned short offset, unsigned short sequence);

/* process the received acknowledgement for small file*/
void parseACK_small(char *recvACK);

/* Send file to target when the size of file is larger than 1MB */
void sendFile_big();

/* construct the first packet that contains information of the big file,
 * including file length, segment length, MD5 of all segments */
void getInfoPacket(char *packet);

/* Construct the packet for big file, including segment number and sequence number */
void getPacket_big(char *packet, char *payload, unsigned int segNumber[], unsigned int segNumLen, unsigned short sequence);

/* process the received acknowledgement for big file */
void parseACK_big(char *recvACK);

/* clean up used memory after finishing transmission */
void cleanup();
