#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	unsigned int ip=0;
	unsigned short port=0;
	string filename;

	for(int i=0; i<argc; i++)
	{
		string arg = string(argv[i]);
		if(arg.compare("-r"))
		{
			//receiver's information
			i++;
			getIPAddress(&argv[i], &ip, &port);
		}
		else if(arg.compare("-f"))
		{
			//file's information
			i++;
			filename = string(argv[i]);
		}
		else
		{
			cout << "invalid arguments: "+argv[i];
		}
	}
}

void getIPAddress(char *ip[], unsigned int *IPAddress, unsigned short *port)
{
	unsigned short temp = 0;
	unsigned short index = 0;
	int i;

	//calculate ip
	for(i=0; i<strlen(*ip); i++)
	{
		char c = *ip[i];
		if(c == '.')
		{
			if(temp>=0 && temp <= 255)
			{
				*IPAddress = *IPAddress | temp << index * 8;
				index ++;
			}
			else
			{
				cout << "invalid IP address";
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
	}

	//calculate port
	*port = atoi(*(ip+i));//if atoi not successful, what will happen?
}