#include "global.h"

using namespace std;

unsigned int getIPAddress(char ip[]);
unsigned short getPortNumber(string port);

int main(int argc, char *argv[])
{
        unsigned int ip=0;
        unsigned short portNumber=0;
        string filename;
        int i;

        if (argc < 5) {
                cout << "Invalid command line arguments. \nThe input should have at least five arguments.\n";
                exit(0);
        }

        for(i=1; i<argc; i++)
        {
                string arg = string(argv[i]);
                if(arg.compare("-r")==0)
                {
                        //receiver's information
                        i++;
                        ip = getIPAddress(argv[i]);
						portNumber = getPortNumber(argv[i]);
                }
                else if(arg.compare("-f")==0)
                {
                        //file's information
                        i++;
                        filename = string(argv[i]);
                }
                else
                {
                        printf("invalid arguments: %s\n", argv[i]);
                }
        }

        printf("IP: %x, port: %d\nfilename:", ip, portNumber);

        cout << filename << "\n";
}

unsigned short getPortNumber(string port)
{
	int colonIndex = port.find(":");
	port = port.substr(colonIndex+1);
	unsigned short portNumber = 0;

	for(int i=0; i<port.length(); i++)
	{
		char c = port[i];
		if(c>='0' && c<='9')
		{
			portNumber *= 10;
			portNumber += (c - '0');
		}
		else
		{
			cout << "Illegal port number" + port;
		}
	}
	return portNumber;
}

unsigned int getIPAddress(char ip[])
{
		unsigned int IPAddress = 0;
        unsigned short temp = 0;
        unsigned short index = 0;
        int i;
        int len = strlen(ip);

        //calculate ip
        for(i=0; i < len; i++)
        {
                char c = ip[i];
                if(c == '.')
                {
                        if(temp>=0 && temp <= 255)
                        {
                                IPAddress = IPAddress | temp << index * 8;
                                index ++;
                                temp = 0;
                        }
                        else
                        {
                                cout << "invalid IP address\n";
                                return -1;
                        }
                }
                else if(c >= '0' && c <= '9')
                {
                        temp = temp*10 + (c-'0');
                }
                else if(c == ':')
                {
                        break;
                }
                else
                {
                        cout<<"invalid IP address";
                        return -1;
                }
        }

        if(temp>=0 && temp <= 255)
        {
                IPAddress = IPAddress | temp << index * 8;
                index ++;
        }
        else
        {
                cout << "invalid IP address";
                return -1;
        }

        if (index != 4)
                return -1;

        return IPAddress;
}
