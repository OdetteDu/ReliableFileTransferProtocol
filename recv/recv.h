#include <fstream>

/* Interpret flags in command line, to get information about source machine */
bool parseFlag(int argc, char *argv[], struct sockaddr_in *sin);

/* send acknowledgement back to sender */
bool returnACK(int sock, struct sockaddr *sin, unsigned int acknowledge);

/* Start to receive file, return true if succeed */
void engage(int sock_num, struct sockaddr *sock_recv);

/* Start to receive a big file whose size is larger than 1MB */
bool recv_big(int sock, struct sockaddr *sin, FILE *fp, unsigned long long length);
