#include "../global.h"
#include "send.h"

static int sock;
static bool *hasACK;
static map<unsigned int, char*> store;
static map<unsigned int, unsigned short> pck_len;
static sockaddr *sin_send;
static sockaddr *sin_recv;

void *send_thread(void *argv) {
	bool sending = true;
	while (sending) {
		sending = false;
		for (map<unsigned int, char*>::iterator it = store.begin(); it != store.end(); it++) {
			// send all packets that haven't been acknowledged
			if (!hasACK[it->first]) {
				sending = true;
				// printf("send packet: offset: %d, length: %d\n", (int)(it->first), (int)(pck_len[it->first]));
				if (sendto(sock, it->second, pck_len[it->first], 0, sin_send, sizeof(struct sockaddr)) < 0) {
					if (errno == EAGAIN) {
						// congestion occurs
						printf("congestion occurs.\n");
					}
					else {
						fprintf(stderr, "*Error* network error, transmission aborted.\n");
						return NULL;
					}
				}
			}
		}
	}
}

/* process the received acknowledgement for small file
 * return true if succeed */
bool parseACK_small(char *recvACK)
{
	uint8_t r[16];
	md5((uint8_t *)(recvACK+16), 4, r);
	if (packetCorrect((uint8_t *) recvACK, r)) {
		hasACK[ntohl(*(unsigned int*)(recvACK + 16))] = true;
		return true;
	}
	else
		return false;
}

/* start to send file, return true if succeed */
bool engage_small(int sock_num, struct sockaddr *sock_send, struct sockaddr *sock_recv,
	unsigned short largestOffset, map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length)
{
	int i, rc;
	bool isACK[largestOffset+1];
	for (i = 0; i < largestOffset + 1; i++)
		isACK[i] = false;
	sock = sock_num;
	hasACK = isACK;
	store = *storage;
	pck_len = *pck_length;
	sin_send = sock_send;
	sin_recv = sock_recv;

	pthread_t send_tid;
	rc = pthread_create(&send_tid, NULL, send_thread, NULL);
	if (rc != 0) {
		fprintf(stderr, "*Error* cannot create a new thread to send packets.\n");
		return false;
	}

	bool mayComplete = false;
	bool complete = false;
	int numACK = 0;
	int recvCount;
	char *recvBuf = (char*)malloc(sizeof(char) * 20);
	unsigned int sockaddr_len = sizeof(struct sockaddr);
	while (true) {
		recvCount = recvfrom(sock, recvBuf, 20, 0, sin_recv, &sockaddr_len);
		if (recvCount == 20 && parseACK_small(recvBuf)) {
			numACK++;
			if (numACK >= largestOffset)
				mayComplete = true;
		}
		
		if (mayComplete) {
			// start to check whether the transmission is completed
			complete = true;
			for (i = 0; i < largestOffset + 1; i++)
				if (!hasACK[i]) {
					complete = false;
					break;
				}

			if (complete) break;
		}
	}

	delete recvBuf;
	pthread_join(send_tid, NULL);

	return complete;
}
