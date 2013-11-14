#include "../global.h"
#include "send.h"

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

/* Send file to target when the size of file is not larger than 1MB */
void readFile_small(FILE* fp, map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length)
{
	char c;
	unsigned short countSize;
	unsigned short offset = 0;
	unsigned short maxPackageSize = 2048;
	char *packet;

	while(true){
		countSize = 0;
		
		char data[maxPackageSize];
		while (maxPackageSize > countSize && (c = fgetc(fp)) != EOF) {
			data[countSize] = c;
			countSize++;
		}
		// carefully check whether the the packet size is times of 2048
		if ((c = fgetc(fp)) != EOF)
			fseek(fp, -1, SEEK_CUR);

		packet = (char*)malloc(sizeof(char) * (countSize + 20));
		getPacket_small(packet, data, countSize, offset, (c==EOF) ? 1 : 0);
		
		storage->insert(pair<unsigned int, char*>(offset, packet));
		pck_length->insert(pair<unsigned int, unsigned short>(offset, countSize));
		
		offset++;
		if (c == EOF)
			break;
	}
}

/* process the received acknowledgement for small file*/
void parseACK_small(char *recvACK, set<unsigned int> *ACK)
{
	uint8_t r[16];
	md5((uint8_t *) recvACK, 4, r);
	if (packetCorrect((uint8_t *) recvACK, r))
		ACK->insert(ntohl(*(unsigned int*)(recvACK + 16)));
}
