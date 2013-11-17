#include <fstream>

/* Interpret flags in command line, to get information about source machine */
bool parseFlag(int argc, char *argv[], struct sockaddr_in *sin);

/* Start to receive file, return true if succeed */
void engage(int sock_num, struct sockaddr *sock_recv);
