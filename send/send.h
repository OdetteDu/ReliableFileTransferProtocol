
/* get the size of file */
unsigned long long getSize(char *path);

/* get file name, return the length of name */
unsigned int getName(char *path, char *name);

/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[], struct sockaddr_in *sin, char **filepath);

/* Send file to target when the size of file is not larger than 1MB
 * header can distinguish last packet with others, and the last packet only contains file name
 * Return the largest offset (in unit of 2KB) */
int readFile_small(FILE* fp, char *filename, unsigned int totalLength,
	map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length);

/* start to send small file, return true if succeed */
bool engage_small(int sock_num, struct sockaddr *sock_send, struct sockaddr *sock_recv,
	unsigned short largestOffset, map<unsigned int, char*> *storage, map<unsigned int, unsigned short> *pck_length);

/* start to send big file, return true if succeed */
bool engage_big(int sock_num, struct sockaddr *sock_send, struct sockaddr *sock_recv, FILE* fp, unsigned long long fLen);
