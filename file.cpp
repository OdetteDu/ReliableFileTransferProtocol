#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

char *contents = (char*) malloc(sizeof(char)*1024*1024); //should be global

void save(string filename, bool largeFile, bool lastPacket, unsigned int length, unsigned int packetIndex, char *payload)
{
	if(!largeFile)
	{
		memcpy(content + 2048 * packetIndex, payload, length);
		/*
		for(int i=0; i<length; i++)
		{
			contents[2048*packetIndex+i] = payload[i];
		}*/

		if(lastPacket)
		{
			ofstream fs;
			fs.open(filename+".recv");
			fs.write(contents, 2048 * packetIndex + length);
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
