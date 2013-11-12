#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


void save(string filename, bool largeFile, bool lastPacket, unsigned int length, unsigned int packetIndex, char *payload[])
{
	if(!largeFile)
	{
		char *contents[] = malloc(sizeof(char)*1024*1024);//should be global

		for(int i=0; i<length; i++)
		{
			contents[2048*packetIndex+i] = payload[i];
		}

		if(lastPacket)
		{
			ofstream fs;
			fs.open(filename+".recv");
			fs << contents;
			fs.close();
		}
	}
	else
	{
		ofstream fs;
		fs.open(filename+".recv");
		fs.seekp(2048*packetIndex);
		fs << payload;
		fs.close();
	}
}