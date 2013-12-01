#include "global.h"
#include "send/send.h"

#define SMALL_SIZE 1048576

int main(int argc, char *argv[])
{
	FILE *fp;
	char *path = NULL;
	char *filename = NULL;
	unsigned long long fileSize = 0;

	int sock;
	struct sockaddr_in sin_send;
	struct sockaddr_in sin_recv;
	struct timeval tv;

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	/* parse command line arguments and prepare socket info to send data */
	if (parseFlag(argc, argv, &sin_send, &path)) {
		if ((fp = fopen(path, "rb")) != NULL) {
			fileSize = getSize(path);
			filename = (char*)malloc(sizeof(char) * (strlen(path) + 1));
			getName(path, filename);
		}
		else {
			fprintf(stderr, "Fail to open the file!\n");
			exit(-1);
		}
	}
	else
		exit(-1);
	
	/* establish the socket */
	// create UDP socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		fprintf(stderr, "*Error* unknown error occurs when opening UDP socket.\n");
		exit(-1);
	}

	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval)) < 0) {
		fprintf(stderr, "*Error* cannot set time out for receiving.\n");
		exit(-1);
	}

	// construct receiving socket info
	memset(&sin_recv, 0, sizeof(struct sockaddr_in));
	sin_recv.sin_family = AF_INET;
	sin_recv.sin_addr.s_addr = INADDR_ANY;
	sin_recv.sin_port = sin_send.sin_port;
	
	// bind the socket to specified port
	if (bind(sock, (struct sockaddr*) &sin_recv, sizeof(sin_recv)) < 0) {
		fprintf(stderr, "*Error* cannot bind socket to the port.\n");
		exit(-1);
	}

	bool complete = false;
	printf("[send data] start (%llu)\n", fileSize);
	if (fileSize <= SMALL_SIZE) {
		// initialize data structures to store all packets
		map<unsigned int, char*> storage;
		map<unsigned int, unsigned short> pck_length;
		unsigned short largestOffset = readFile_small(fp, filename, fileSize, &storage, &pck_length);
		
		// engage sending
		complete = engage_small(sock, (struct sockaddr*) &sin_send, (struct sockaddr*) &sin_recv, largestOffset,
			&storage, &pck_length);
		
		// free all memory resources in temporary storage
		for (map<unsigned int, char*>::iterator it = storage.begin(); it != storage.end(); it++)
			delete it->second;
	}
	else
		complete = engage_big(sock, (struct sockaddr*) &sin_send, (struct sockaddr*) &sin_recv, fp, filename, fileSize);

	if (complete)
		printf("[completed]\n");
	else
		printf("[incomplete] Error or timeout occurs.\n");

	fclose(fp);
	close(sock);
	delete filename;
	return 0;
}
