#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/wait.h>

void printError(char *error) {
	printf("***Error***\n\t%s\n", error);
}

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in sin;
	unsigned short server_port = -1;
	char *buf;
	int i, count;
	socklen_t addrlen;
	int status;
	
	if (argc < 2) {
		printError("Must specify port number in command line.");
		exit(0);
	}
	else
		server_port = atoi(argv[1]);

	// create UDP socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		printError("unknown error occurs when opening UDP socket.");
		exit(0);
	}

	// construct send buffer
	if (!(buf = (char*)malloc(sizeof(char) * 26))) {
		printError("cannot allocate memory to buffer.");
		exit(0);
	}

	// set address information
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(server_port);
	addrlen = sizeof(sin);

	if (bind(sock, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		printError("cannot bind socket to the port.");
		exit(0);
	}

	if (fork() == 0) {
		// receive UDP packet
		while (true) {
			count = recvfrom(sock, buf, 26, 0, (struct sockaddr*) &sin, &addrlen);

			if (count < 0) 
				perror("Receive packet fails.");
			else {
				printf("Receive %d bytes.\nContent: ", count);
				for (i = 0; i < count; i++)
					printf("%c", buf[i]);
				printf("\n");
			}
		}
	}
	else {
		while (true) {
			sleep(10);
			printf("waiting...\n");
		}
		wait(&status);
	}

	delete buf;
	close(sock);
	return 0;
}
