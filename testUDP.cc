#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>

void printError(char *error) {
	printf("***Error***\n\t%s\n", error);
}

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in sin;
	struct hostent *host = gethostbyname("cai.cs.rice.edu");
	unsigned int server_addr = *(unsigned int*) host->h_addr_list[0];
	unsigned short server_port = -1;
	char *buf;
	int i, count;
	
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

	for (i = 0; i < 26; i++)
		buf[i] = 'a' + i;

	printf("server address: %x\n", server_addr);

	// set address information
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = server_addr;
	sin.sin_port = htons(server_port);

	// send the UDP packet
	count = sendto(sock, buf, 26, 0, (struct sockaddr*) &sin, sizeof(sin));

	if (count < 0) 
		perror("Send packet fails.");
	else 
		printf("Send %d bytes.\n", count);
	
	close(sock);
	delete buf;
	return 0;
}
