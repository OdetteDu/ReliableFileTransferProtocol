#include "global.h"
#include "SendFile/send.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *fp1;

/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[])
{
}

/* Send file to target when the size of file is not larger than 1MB */
void readFile_small()
{
	char c;
	unsigned short countSize;
	unsigned short offset = 0;
	unsigned short maxPackageSize = 2048;
	char *packet = (char*)malloc(sizeof(char) * (countSize + 20));

	while(true){
		countSize = 0;
		
		char data[maxPackageSize];
		while (maxPackageSize > countSize && (c = fgetc(fp1)) != EOF) {
			data[countSize] = c;
			countSize++;
		}
		
		getPacket_small(packet, data, countSize, offset, (c==EOF) ? 1 : 0);
		
		//TODO: store this packet in somewhere for future use

		offset++;
		if (c == EOF)
			break;
	}
}

/* Construct the small packet, including MD5, length of payload, offset in the
 * whole file and sequence number */
void getPacket_small(char *packet, char *payload, unsigned short payloadLen, unsigned short offset, unsigned short status)
{
	unsigned int header;
	
	if(status == 1)
		header = (payloadLen<<20)|(offset<<8)|(0x3);
	else
		header = (payloadLen<<20)|(offset<<8)|(0x1);
	
	*(unsigned int*)(packet+16) = htonl(header);
	
	md5((uint8_t *) (packet+16), payloadLen, (uint8_t *) packet);
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

//open the Files
bool OpenFiles(const char * ASourceName){
	if((fp1 = fopen(ASourceName, "rb"))==NULL){
		printf("There is something wrong when reading the source file.\n");
		return false;
	}
	return true;
}

//close the files
void CloseFiles(){
	if(fclose(fp1) == 0){
		printf("The source file close successfully\n");
	}else {
		printf("Can't close the source file\n");
	}
}
/* main of sendfile */
int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Missing file name.\n");
		exit(0);
	}
	else
		OpenFiles(argv[1]);
	
	readFile_small();
	CloseFiles();
	return 0;
}
