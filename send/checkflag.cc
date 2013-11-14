#include "../global.h"
#include "send.h"

bool getIPandPort(char *in, struct sockaddr_in *sin)
{
	unsigned int IPAddress = 0;
	unsigned int port = 0;
        short temp = 0;
        short index = 3;
        int len = strlen(in), i;
	char c;

        for(i=0; i < len; i++)
        {
                c = in[i];
                if(c == '.')
                {
                        if(temp>=0 && temp <= 255)
                        {
                                IPAddress = IPAddress | temp << index * 8;
                                index --;
                                temp = 0;
                        }
                        else
                        {
                                printf("invalid in address\n");
                                return false;
                        }
                }
                else if(c >= '0' && c <= '9')
                {
                        temp = temp*10 + (c-'0');
                }
                else if(c == ':')
                        break;
                else
                {
                        printf("invalid in address\n");
                        return false;
                }
        }

        if (temp>=0 && temp <= 255 && index == 0)
                IPAddress = IPAddress | temp << index * 8;
        else
        {
                printf("invalid in address\n");
                return false;
        }

	sin->sin_addr.s_addr = htonl(IPAddress);
	//printf("IP: %x\n", htonl(IPAddress));

	for(i++; i < len; i++)
	{
		c = in[i];
		if(c>='0' && c<='9')
		{
			port *= 10;
			port += (c - '0');
			if (port > 65535) {
				printf("Port number must be smaller than 65536.\n");
				return false;
			}
		}
		else
		{
			printf("Illegal port number\n");
			return false;
		}
	}

	sin->sin_port = htons((unsigned short)port);
	//printf("port: %d\n", port);

        return true;
}

/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[], struct sockaddr_in *sin, char **filepath) {
	if (argc < 5) {
                printf("Invalid command line arguments. \nThe input should have at least four arguments.\n");
                exit(0);
        }

	memset(sin, 0, sizeof(struct sockaddr_in));
	sin->sin_family = AF_INET;
	
	for(int i=1; i<argc; i++)
        {
                string arg = string(argv[i]);
                if(arg.compare("-r")==0)
                {
                        //receiver's information
                        i++;
                        if (!getIPandPort(argv[i], sin))
				return false;
                }
                else if(arg.compare("-f")==0)
                {
                        //file's information
                        i++;
			*filepath = argv[i];
                }
                else
                {
                        printf("invalid arguments: %s\n", argv[i]);
			return false;
                }
        }

	return true;
}
