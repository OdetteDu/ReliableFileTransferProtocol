#include "../global.h"
#include "send.h"

/* process the received acknowledgement for small file*/
void parseACK_small(char *recvACK, set<unsigned int> *ACK)
{
	uint8_t r[16];
	md5((uint8_t *) recvACK, 4, r);
	if (packetCorrect((uint8_t *) recvACK, r))
		ACK->insert(ntohl(*(unsigned int*)(recvACK + 16)));
}

/* start to send file, return true if succeed */
bool engage_small(int sock, struct sockaddr *sin_send, struct sockaddr *sin_recv,
	unsigned short largestOffset, map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length)
{
}
