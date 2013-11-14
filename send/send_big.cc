#include "../global.h"
#include "send.h"

/* Send file to target when the size of file is larger than 1MB */
void sendFile_big()
{
}

/* construct the first packet that contains information of the big file,
 * including file length, segment length, MD5 of all segments */
void getInfoPacket(char *packet)
{
}

/* Construct the packet for big file, including segment number and sequence number */
void getPacket_big(char *packet, char *payload, unsigned int segNumber[], unsigned int segNumLen, unsigned short sequence)
{
}

/* process the received acknowledgement for big file */
void parseACK_big(char *recvACK)
{
}
