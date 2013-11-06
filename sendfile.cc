#include "global.h"
#include "SendFile/send.h"

/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[])
{
}

/* Send file to target when the size of file is not larger than 1MB */
void sendFile_small()
{
}

/* Construct the small packet, including MD5, length of payload, offset in the
 * whole file and sequence number */
void getPacket_small(char *packet, char *payload, unsigned short payloadLen, unsigned short offset, unsigned short sequence)
{
}

/* process the received acknowledgement for small file*/
void parseACK_small(char *recvACK)
{
}

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

/* clean up used memory after finishing transmission */
void cleanup()
{
}

/* main of sendfile */
int main(int argc, char *argv[])
{
	printf("Now this program is simply for testing the MD5 function.\n");
	if (argc < 2) {
		printf("<usage> %s [input_string]\n", argv[0]);
		exit(0);
	}

	uint8_t digest[16];
	int i;
	md5((uint8_t *) argv[1], strlen(argv[1]), digest);
	
	printf("MD5: ");
	for (i = 0; i < 16; i++)
		printf("%2.2x", digest[i]);
	printf("\n");

	return 0;
}
