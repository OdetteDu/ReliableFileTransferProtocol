#include "global.h"
#include "send/send.h"

#define SMALL_SIZE 1048576

void testing(map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *length) {
	char *pck;
	int i, index = 0;
	unsigned int header;
	for (map<unsigned int, char*>::iterator it = storage->begin(); it != storage->end(); it++) {
		printf("===No. %d===\n", ++index);
		pck = it->second;
		printf("MD5:\n");
		for (i = 0; i < 16; i++)
			printf("%2.2x", *((uint8_t*)pck+i));
		printf("\n");
		header = ntohl(*(unsigned int*)(pck + 16));
		printf("offset: %d\nlength: %d\nstatus: %d\n", (header>>8) & 0xfff, (header>>20) & 0xfff, header & 0x3);
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char *path = NULL;
	char *filename = NULL;
	unsigned long long fileSize = 0;

	int sock;
	struct sockaddr_in sin_send;
	struct sockaddr_in sin_recv;
	
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

	if (fileSize <= SMALL_SIZE) {
		// initialize data structures to store all packets
		map<unsigned int, char*> storage;
		map<unsigned int, unsigned short> pck_length;
		bool complete = false;
		unsigned short largestOffset = readFile_small(fp, filename, &storage, &pck_length);
		
		// engage sending
		printf("[send data] start (%llu)\n", fileSize);
		complete = engage_small(sock, (struct sockaddr*) &sin_send, (struct sockaddr*) &sin_recv, largestOffset,
			&storage, &pck_length);
		if (complete)
			printf("[completed]\n");

		testing(&storage, &pck_length);		// TODO: for testing, will remove
		
		// free all memory resources in temporary storage
		for (map<unsigned int, char*>::iterator it = storage.begin(); it != storage.end(); it++)
			delete it->second;
	}
	else {
		// TODO: prepare to send a large file
	}

	fclose(fp);
	close(sock);
	delete filename;
	return 0;
}
