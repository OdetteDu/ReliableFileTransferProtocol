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
void sendFile_small()
{
	int countSize = 0;
	while(true){
	char c;
	int maxPackageSize = 2048;
	char data[maxPackageSize+1];
		while ((c = fgetc(fp1))!=EOF && maxPackageSize != 0){
			data[countSize%maxPackageSize] = c;
			maxPackageSize --;
			countSize ++;
		}
		data[countSize%maxPackageSize] = '\0';
		char *thepacket = (char*)malloc(sizeof(char)*(countSize%maxPackageSize+20));
		getPacket_small(thepacket, data.length, countSize);
		//to send the packet codes here---
	}
}

/* Construct the small packet, including MD5, length of payload, offset in the
 * whole file and sequence number */
void getPacket_small(char *packet, char *payload, unsigned short payloadLen, unsigned short offset)
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

//open the Files
bool OpenFiles(const char * ASourceName){
	if((fp1 = fopen(ASourceName, "r"))==NULL){
		printf("There is something wrong when reading the source file.\n");
		return false;
	}
	return true;
}

//close the files
void CloseFiles(){
	if(fclose(fp1)==0){
		printf("The source file close successfully\n");
	}else {
		printf("Can't close the source file\n");
	}
}
/* main of sendfile */
int main(int argc, char *argv[])
{
	OpenFiles(argv[0]);
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
