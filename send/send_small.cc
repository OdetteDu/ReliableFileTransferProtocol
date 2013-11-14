#include "../global.h"
#include "send.h"

/* process the received acknowledgement for small file
 * return true if succeed */
bool parseACK_small(char *recvACK, bool isACK[])
{
	uint8_t r[16];
	md5((uint8_t *) recvACK, 4, r);
	if (packetCorrect((uint8_t *) recvACK, r)) {
		isACK[ntohl(*(unsigned int*)(recvACK + 16))] = true;
		return true;
	}
	else
		return false;
}

/* start to send file, return true if succeed */
bool engage_small(int sock, struct sockaddr *sin_send, struct sockaddr *sin_recv,
	unsigned short largestOffset, map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length)
{
	map<unsigned int, char*> store = *storage;
	map<unsigned int, unsigned short> pck_len = *pck_length;
	bool hasACK[largestOffset+1];
	bool mayComplete;
	int numACK = 0;
	int i;
	int status = 0;
	for (i = 0; i < largestOffset + 1; i++)
		hasACK[i] = false;

	if (fork() == 0) {
		// child: sending process
		while (true) {
			for (map<unsigned int, char*>::iterator it = store.begin(); it != store.end(); it++) {
				if (!hasACK[it->first]) {
					printf("send packet: offset: %d, length: %d\n", (int)(it->first), (int)(pck_len[it->first]));
					if (sendto(sock, it->second, pck_len[it->first], 0, sin_send, sizeof(struct sockaddr)) < 0) {
						if (errno == EAGAIN) {
							// congestion occurs
							printf("congestion occurs.\n");
						}
						else {
							printf("*Error* network error, transmission aborted.\n");
							return false;
						}
					}
				}
			}
		
			sleep(10);
		}
	}
	else {
		// parent: receiving process
		wait(&status);
	}

	return true;
}
