#include <errno.h>
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
#include <fcntl.h>
#include <map>
#include <set>

#define SMALL_SIZE 1048576

/* function to get MD5 digest of message */
void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);
