#include "../global.h"
#include "send.h"
#include "semaphore.h"

#define WINDOW_SIZE 1000

static int sock;
static sem_t mutex;
static sockaddr *sin_send;
static sockaddr *sin_recv;
static bool stop;

struct sendQueue;
struct sendQueue {
	unsigned int seq;
	char* pck;
	bool hasACK;
	struct sendQueue *next;
};
struct sendQueue *head;

/* get a packet from specific position in a file */
char* getBigPacket(FILE *fp, unsigned int offset, unsigned short length, int status){
    char* packet;
    unsigned int header;
    
    packet = (char*)malloc(sizeof(char) * (length + 20));
    
    fseek(fp, offset*2048, SEEK_SET);
    fread(packet + 20, 1, length, fp);
    
    if(status == 1)
        header = (offset<<2)|(0x2);
    else
        header = (offset<<2)|(0x0);
    
    *(unsigned int*)(packet+16) = htonl(header);
    
    md5((uint8_t *) (packet+16), length+4, (uint8_t *) packet);
    
    return packet;
}

void *send_thread_big(void *argv) {
	bool sending = true;
	while (sending) {
		if (stop)
			return NULL;
		sem_wait(&mutex);
		// send packet
		sem_pose(&mutex);
		sending = false;
	}
}

/* check the received acknowledgement for big file */
bool parseACK_big(char *recvACK)
{
	uint8_t r[16];
	md5((uint8_t *)(recvACK+16), 4, r);
	return packetCorrect((uint8_t *) recvACK, r);
}

/* start to send big file, return true if succeed */
bool engage_big(int sock_num, struct sockaddr *sock_send, struct sockaddr *sock_recv, FILE* fp, unsigned long long fLen) {
	unsigned int minWait = 0, maxWait = (unsigned int)(fLen / 2048);
	unsigned short lastpckLength = (unsigned short)(fLen % 2048);
	if (lastpckLength == 0) {
		lastpckLength = 2048;
		maxWait--;
	}

	int win_index = 0, i;
	bool isACK[WINDOW_SIZE];
	for (i = 0; i < WINDOW_SIZE; i++)
		isACK[i] = false;
	sock = sock_num;
	sin_send = sock_send;
	sin_recv = sock_recv;
	stop = false;

	/* semaphore to prevent race between sending thread and main */
	sem_init(&mutex, 0 , 1);

	pthread_t send_tid;
	int rc = pthread_create(&send_tid, NULL, send_thread_small, NULL);
	if (rc != 0) {
		fprintf(stderr, "*Error* cannot create a new thread to send packets.\n");
		return false;
	}

	bool complete = false;
	int timeoutCount = 1;
	int recvCount;
	char *recvBuf = (char*)malloc(sizeof(char) * 20);
	unsigned int sockaddr_len = sizeof(struct sockaddr);
	while (!complete) {
		recvCount = recvfrom(sock, recvBuf, 20, 0, sin_recv, &sockaddr_len);
		if (recvCount < 0) {
			fprintf(stderr, "Time out occurs... (no ACK in %d secondes)\n", (timeoutCount++)*5);
			if (timeoutCount > 10) {
				stop = true;
				break;
			}
			else continue;
		}
		else if (recvCount == 20 && parseACK_big(recvBuf)) {
			unsigned int seq = ntohl(*(unsigned int*)(recvBuf+16));
			if (seq == maxWait + 1) {
				complete = true;
			}
			else if (seq == minWait) {
				// find next minWait
				sem_wait(&mutex);
				// update packet length
				sem_post(&mutex);
			}
		}
		timeoutCount = 0;
	}

	delete recvBuf;
	pthread_join(send_tid, NULL);

	return complete;
}
