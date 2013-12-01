#include "../global.h"
#include "recv.h"

#define WINDOW_SIZE 1000
#define MAX_PAYLOAD 2048

static bool *hasACK;
static unsigned int minWait;
static unsigned int maxWait;
static unsigned int lastPacketLength;
static int minIndex;

/* process a received packet */
void recvBigPack(int sock, struct sockaddr *sin, FILE * fp, char * pck){
	unsigned int fifthLine = ntohl(*(unsigned int *)(pck + 16));
	unsigned int size = (fifthLine & 0x2) ? lastPacketLength : MAX_PAYLOAD;
	//Check packet integrity with MD5.
	uint8_t result[16];
	md5((uint8_t *)(pck+16), size+4, result);
	if (!packetCorrect((uint8_t *)pck, result)) {
		printf("[recv corrupt packet]\n");
		return;
	}
	printf("[recv data] length (%d) ", size);

	// Now that the MD5 is passed, continue to get the sequence number from the header.
	unsigned int seq = fifthLine >> 2;
	returnACK(sock, sin, seq);

	// deal with window
	if (seq < minWait || seq > minWait + WINDOW_SIZE - 1) {
		printf("IGNORED\n");
		return;
	}
	else if (seq == minWait) {
		// slide the window
		hasACK[minIndex] = true;
		while (hasACK[minIndex] && minWait < maxWait) {
        		hasACK[minIndex] = false;
			minWait++;
        		minIndex = (minIndex + 1) % WINDOW_SIZE;
		}		
	}
	else {
		int temp = (minIndex + seq - minWait) % WINDOW_SIZE;
		if (hasACK[temp]) {
			printf("IGNORED\n");
			return;
		}
		else
			hasACK[temp] = true;
	}

	printf("ACCEPTED\n");

	// write data to file
	fseek(fp, (seq-1) * MAX_PAYLOAD, SEEK_SET);
	fwrite(pck+20, 1, size, fp);
}

/* Start to receive a big file whose size is larger than 1MB */
bool recv_big(int sock, struct sockaddr *sin, FILE *fp, unsigned long long length){
	printf("[recv data] start (%llu)\n", length);
	minWait = 1;
	minIndex = 1;
	maxWait = (unsigned int)(length / MAX_PAYLOAD) + 1;
	lastPacketLength = (unsigned int)(length % MAX_PAYLOAD);
	if (lastPacketLength == 0) {
		maxWait --;
		lastPacketLength = 2048;
	}
	bool isACK[WINDOW_SIZE];
	int i;
	for (i = 0; i < WINDOW_SIZE; i++)
		isACK[i] = false;
	hasACK = isACK;

	//start a loop to receive "big" packets
	char *pck = (char*)malloc(sizeof(char)*(MAX_PAYLOAD+20));
	unsigned int sockaddr_len = sizeof(struct sockaddr);
	int count;
	while (true) {
		recvfrom(sock, pck, MAX_PAYLOAD + 20, 0, sin, &sockaddr_len);
		recvBigPack(sock, sin, fp, pck);
		if (minWait == maxWait && hasACK[minIndex]) break;
	}
	
	// sending the "completed" acknowledgement (sequence number of the last packet plus one)
	// multiple sending to compensate potential packet loss
	for (i = 0; i < 20; i++)
		returnACK(sock, sin, maxWait + 1);
	return true;
}
