#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

	unsigned short port=0;

	for(int i=0; i<argc; i++)
	{
		string arg = string(argv[i]);
		if(arg.compare("-p"))
		{
			i++;
			port = atoi(argv[i]);
		}
		else
		{
			cout << "invalid arguments: "+*argv[i];
		}
	}

}