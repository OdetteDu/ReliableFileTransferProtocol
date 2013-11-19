#include "../global.h"
#include "send.h"

/* get the size of file */
unsigned long long getSize(char *path) {
	struct stat fileStat;
	stat(path, &fileStat);
	return (long long)fileStat.st_size;
}

/* get file name, return the length of name */
unsigned int getName(char *path, char *name) {
	unsigned int ret = 0;
	int len = strlen(path);
	char c;
	for (int i = 0; i < len; i++) {
		c = path[i];
		if (c == '/')
			ret = 0;
		else
			name[ret++] = c;
	}
	name[ret] = '\0';
	return ret;
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

	memcpy(packet+20, payload, payloadLen);
	
	md5((uint8_t *) (packet+16), payloadLen+4, (uint8_t *) packet);
}

/* Send file to target when the size of file is not larger than 1MB
 * header can distinguish last packet with others, and the last packet only contains file name
 * Return the largest offset (in unit of 2KB) */
int readFile_small(FILE* fp, char *filename, unsigned int totalLength,
	map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length)
{
	char c;
	unsigned short countSize;
	unsigned short offset = 0;
	unsigned int currentSize = 0;
	unsigned short maxPackageSize = 2048;
	char *packet;

	/* construct all packets into memory */
	while(true){
		countSize = 0;
		
		char data[maxPackageSize];
		while (maxPackageSize > countSize && currentSize < totalLength) {
			c = fgetc(fp);
			currentSize++;
			data[countSize] = c;
			countSize++;
		}
		// carefully check whether the the packet size is times of 2048
		
		packet = (char*)malloc(sizeof(char) * (countSize + 20));
		getPacket_small(packet, data, countSize, offset, 0);
		
		storage->insert(pair<unsigned int, char*>(offset, packet));
		pck_length->insert(pair<unsigned int, unsigned short>(offset, countSize+20));

		offset++;				// increment offset
		if (currentSize == totalLength) break;	// reach the end of file
	}
	
	/* construct a special packet that contains file name */
	unsigned short len_name = strlen(filename);
	packet = (char*)malloc(sizeof(char) * (len_name + 20));
	getPacket_small(packet, filename, len_name, offset, 1);
	storage->insert(pair<unsigned int, char*>(offset, packet));
	pck_length->insert(pair<unsigned int, unsigned short>(offset, len_name + 20));

	return offset;
}
