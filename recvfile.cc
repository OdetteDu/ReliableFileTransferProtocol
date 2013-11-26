#include "global.h"
#include "recv/recv.h"

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in sin_recv;

	/* parse command line arguments and prepare socket info to receive data */
	if (!parseFlag(argc, argv, &sin_recv))
		exit(-1);

	/* establish the socket */
	// create UDP socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		fprintf(stderr, "*Error* unknown error occurs when opening UDP socket.\n");
		exit(-1);
	}

	// bind the socket to specified port
	if (bind(sock, (struct sockaddr*) &sin_recv, sizeof(sin_recv)) < 0) {
		fprintf(stderr, "*Error* cannot bind socket to the port.\n");
		exit(-1);
	}
	
	// start to receive file
	engage(sock, (struct sockaddr*) &sin_recv);

	close(sock);
	return 0;
}
