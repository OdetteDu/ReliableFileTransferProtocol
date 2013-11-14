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

	map<unsigned int, char*> storage;
	map<unsigned int, unsigned short> pck_length;
	
	/* parse command line arguments and prepare socket info to send data */
	if (parseFlag(argc, argv, &sin_send, &path)) {
		if ((fp = fopen(path, "rb")) != NULL) {
			fileSize = getSize(path);
			filename = (char*)malloc(sizeof(char) * (strlen(path) + 1));
			getName(path, filename);
		}
		else {
			printf("Fail to open the file!\n");
			exit(0);
		}
	}
	else
		exit(0);
	
	/* establish the socket */
	// create UDP socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		printf("*Error* unknown error occurs when opening UDP socket.\n");
		exit(0);
	}

	// construct receiving socket info
	memset(&sin_recv, 0, sizeof(struct sockaddr_in));
	sin_recv.sin_family = AF_INET;
	sin_recv.sin_addr.s_addr = INADDR_ANY;
	sin_recv.sin_port = sin_send.sin_port;
	
	// bind the socket to specified port
	if (bind(sock, (struct sockaddr*) &sin_recv, sizeof(sin_recv)) < 0) {
		printf("*Error* cannot bind socket to the port.\n");
		exit(0);
	}

	if (fileSize <= SMALL_SIZE) {
		printf("[send data] start (%llu)\n", fileSize);
		unsigned short largestOffset = readFile_small(fp, filename, &storage, &pck_length);
		engage_small(sock, (struct sockaddr*) &sin_send, (struct sockaddr*) &sin_recv, largestOffset,
			&storage, &pck_length);
	}
	else {
		// TODO: prepare to send a large file
	}

	testing(&storage, &pck_length);

	/* clean up */	
	fclose(fp);
	// free all memory resources in temporary storage
	for (map<unsigned int, char*>::iterator it = storage.begin(); it != storage.end(); it++) {
		delete it->second;
	}
	delete filename;
	return 0;
}
