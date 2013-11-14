#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

unsigned short getPortNumber(string port);

int main(int argc, char *argv[])
{

        unsigned short port=0;

		if(argc < 3)
		{
			cout << "Invalid command line argument.\nThe number of argument has to be 3.\n";
		}

		int i;
        for(i=1; i<argc; i++)
        {
                string arg = string(argv[i]);
                if(arg.compare("-p")==0)
                {
                        i++;
                        port = getPortNumber(argv[i]);
						//printf("Port Number: %d", port);
                }
                else
                {
                        cout << "invalid arguments: "+*argv[i];
                }
        }

		printf("Port Number: %d", port);
		while(true){}
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