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

/* function to get MD5 digest of message */
void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);
