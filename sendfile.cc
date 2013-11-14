#include "global.h"
#include "send/send.h"

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
		printf("offset: %d\n, length: %d\n, status: %d\n", (header>>8) & 0xfff, (header>>20) & 0xfff, header & 0x3);
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char *path = NULL;

	int sock;
	struct sockaddr_in sin;

	map<unsigned int, char*> storage;
	map<unsigned int, unsigned short> pck_length;
	
	/* parse command line arguments and prepare all needed resources */
	if (parseFlag(argc, argv, &sin, &path)) {
		if ((fp = fopen(path, "rb")) != NULL)
			readFile_small(fp, &storage, &pck_length);
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
	
	// bind the socket to specified port
	if (bind(sock, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		printf("*Error* cannot bind socket to the port.\n");
		exit(0);
	}

	testing(&storage, &pck_length);

	/* clean up */	
	fclose(fp);
	// free all memory resources in temporary storage
	for (map<unsigned int, char*>::iterator it = storage.begin(); it != storage.end(); it++) {
		delete it->second;
	}
	return 0;
}
