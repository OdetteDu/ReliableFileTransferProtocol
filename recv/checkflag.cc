#include "../global.h"
#include "recv.h"

bool parseFlag(int argc, char *argv[], struct sockaddr_in *sin)
{
	int port;

	if(argc < 3)
	{
		fprintf(stderr, "<usage> %s -p <receiving port>\n", argv[0]);
		fprintf(stderr, "*Note* Only the first two arguments will be interpreted.\n");
		return false;
	}
	
        string arg = string(argv[1]);
        if(arg.compare("-p")==0)
	{
		port = atoi(argv[2]);
		if (port >= 18000 && port <= 18200) {
			memset(sin, 0, sizeof(struct sockaddr_in));
			sin->sin_family = AF_INET;
			sin->sin_addr.s_addr = INADDR_ANY;
			sin->sin_port = htons(port);
			printf("port: %d\n", port);
			return true;
		}
		else {
			fprintf(stderr, "On CLEAR, receiving port number is restricted between 18000 and 18200 inclusive.\n");
			return false;
		}
	}
	else
	{
		fprintf(stderr, "Invalid argument: %s\n", argv[1]);
		return false;
	}
}
