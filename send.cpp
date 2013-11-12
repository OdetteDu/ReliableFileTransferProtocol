#include "global.h"

using namespace std;

bool getIPAddress(char ip[], unsigned int *IPAddress, unsigned short *port);

int main(int argc, char *argv[])
{
	unsigned int ip=0;
	unsigned short port=0;
	string filename;
	int i;

	if (argc < 5) {
		cout << "at least 5 arguments.\n";
		exit(0);
	}

	for(i=1; i<argc; i++)
	{
		string arg = string(argv[i]);
		if(arg.compare("-r"))
		{
			//receiver's information
			printf("reading an IP address...%d\n", i);
			i++;
			getIPAddress(argv[i], &ip, &port);
		}
		else if(arg.compare("-f"))
		{
			//file's information
			printf("reading file name...%d\n", i);
			i++;
			filename = string(argv[i]);
		}
		else
		{
			printf("invalid arguments: %s\n", argv[i]);
		}
	}

	printf("ip: %x, port: %d\nfilename:", ip, port);

	cout << filename << "\n";
}

bool getIPAddress(char ip[], unsigned int *IPAddress, unsigned short *port)
{
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
				*IPAddress = *IPAddress | temp << index * 8;
				index ++;
				temp = 0;
			}
			else
			{
				cout << "invalid IP address\n";
				return false;
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
			return false;
		}
	}

	if(temp>=0 && temp <= 255)
	{
		*IPAddress = *IPAddress | temp << index * 8;
		index ++;
	}
	else
	{
		cout << "invalid IP address";
		return false;
	}

	if (index != 4)
		return false;

	//calculate port
	*port = atoi(ip+i);//if atoi not successful, what will happen?
	return true;
}
