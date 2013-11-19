#include "../global.h"
#include "recv.h"

#define SMALL_SIZE 1048576

// send back an acknowledge packet, after successfully receiving the data
// return true if succeed
bool returnACK(int sock, struct sockaddr *sin, unsigned int acknowledge){
	//initialize the header (only contains ACK number in this case)
	char toSend[20];
	*(unsigned int*)(toSend + 16) = htonl(acknowledge);
	//calculate an MD5 for the acknowledge number
	md5((uint8_t *)(toSend + 16), 4, (uint8_t *)toSend);

	if (sendto(sock, toSend, 20, 0, sin, sizeof(struct sockaddr)) < 0) {
		if (errno == EAGAIN) {
			// congestion occurs
			printf("congestion occurs.\n");
		}
		else {
			fprintf(stderr, "*Error* network error, transmission aborted.\n");
			return false;
		}
	}

	return true;
}

/* receiving a small file */
bool receive_small(int sock, struct sockaddr *sin, char *pck){
	unsigned int sockaddr_len = sizeof(struct sockaddr);
	set<unsigned int> hasACK;
	unsigned int largestOffset = -1;
	int fileLength = -1;
	char *content = (char*)malloc(sizeof(char) * SMALL_SIZE);
	char *fileName = NULL;
	bool receiving = true;
	bool fileNameReceived = false;

	while (receiving) {
		// header: interger value on fifth line of the data packet
		unsigned int fifthLine = ntohl(*(unsigned int *)(pck + 16)); 
    		int size = (fifthLine >> 20) & 0xFFF;
		// Since the data packet sent will never exceed 2048 byte,
		// there must be an error occurred if size is greater than 2048
    		if (size <= 2048) {
			// sequence number (also is offset) from the fifth line of the packet
			int seq = (fifthLine >> 8) & 0xFFF;
			// check the correctness of packet
			uint8_t result[16];
			md5((uint8_t *)(pck+16), size+4, result);
			if (packetCorrect((uint8_t *)pck, result)) {
				if (hasACK.find(seq) == hasACK.end()) {
					// a new packet arrives
					if (fifthLine & 0x2) {
						fileNameReceived = true;
						largestOffset = seq;
						fileName = (char*)malloc(sizeof(char) * (size+6));
						memcpy(fileName, pck+20, size);
						fileName[size] = '\0';
						strcat(fileName, ".recv");
					}
					else {
						memcpy(content + seq*2048, pck + 20, size);
						if (seq * 2048 + size > fileLength)
							fileLength = seq * 2048 + size;
					}
									
					hasACK.insert(seq);
				}
				
				// send acknowledge to sender
				if (!returnACK(sock, sin, seq))
					break;

				// check complete status if the last packet (file name packet) is received
				if (fileNameReceived) {
					receiving = false;
					for (int i = 0; i <= largestOffset; i++)
						if (hasACK.find(i) == hasACK.end()) {
							receiving = true;
							break;
						}
				}
			}  // endif(packetCorrect): if packet is corrupted, nothing will be useful in it
		} // end if (size<=2048): no length of payload will exceed 2048

		// get another packet
		if (receiving) recvfrom(sock, pck, 2068, 0, sin, &sockaddr_len);
	}

	if (!receiving) {
		ofstream fs;
		fs.open(fileName, ofstream::out | ofstream::trunc);
		fs.write(content, fileLength);
		printf("file length: %d\n", fileLength);
		fs.close();
	}

	delete content;
	if (fileName != NULL)
		delete fileName;
	return (!receiving);
}

/* Start to receive file, return true if succeed */
void engage(int sock, struct sockaddr *sin) {
	unsigned int sockaddr_len = sizeof(struct sockaddr);
	char *recvBuf = (char*)malloc(sizeof(char) * 2068);
	bool firstArrived = false;

	// waiting for the first (correct) packet to arrive and determine the receive procedure
	while (!firstArrived) {
		int count = recvfrom(sock, recvBuf, 2068, 0, sin, &sockaddr_len);
		// because of the header design, no packet has a size less than 21.
		if (count > 20) {
			// check the correctness of packet
			uint8_t result[16];
    			md5((uint8_t *)(recvBuf+16), count-16, result);
			if (packetCorrect((uint8_t *)recvBuf, result))
				firstArrived = true;
		}
	}

	unsigned int header = ntohl(*(unsigned int*)(recvBuf+16));
	if (header & 0x1) {
		// mode: small file transmission (not larget than 1MB)
		if (receive_small(sock, sin, recvBuf))
			printf("[completed]\n");
	}
	else {
		// TODO: mode: large file transmission
	}

	delete recvBuf;
}
