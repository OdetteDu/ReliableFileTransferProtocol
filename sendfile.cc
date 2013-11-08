#include "global.h"
#include "SendFile/send.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *fp1;
/* Interpret flags in command line, to get information about target machine
 * and file that will be transmitted */
bool parseFlag(int argc, char *argv[])
{
}

/* Send file to target when the size of file is not larger than 1MB */
void sendFile_small()
{
}

/* Construct the small packet, including MD5, length of payload, offset in the
 * whole file and sequence number */
void getPacket_small(char *packet, char *payload, unsigned short payloadLen, unsigned short offset, unsigned short sequence)
{
}

/* process the received acknowledgement for small file*/
void parseACK_small(char *recvACK)
{
}

/* Send file to target when the size of file is larger than 1MB */
void sendFile_big()
{
}

/* construct the first packet that contains information of the big file,
 * including file length, segment length, MD5 of all segments */
void getInfoPacket(char *packet)
{
}

/* Construct the packet for big file, including segment number and sequence number */
void getPacket_big(char *packet, char *payload, unsigned int segNumber[], unsigned int segNumLen, unsigned short sequence)
{
}

/* process the received acknowledgement for big file */
void parseACK_big(char *recvACK)
{
}

/* clean up used memory after finishing transmission */
void cleanup()
{
}

//open the Files
bool OpenFiles(const char * ASourceName){
	if((fp1 = fopen(ASourceName, "r"))==NULL){
		printf("There is something wrong when reading the source file.\n");
		return false;
	}
	return true;
}

//close the files
void CloseFiles(){
	if(fclose(fp1)==0){
		printf("The source file close successfully\n");
	}else {
		printf("Can't close the source file\n");
	}
}

//get the next character from the source file
char GetSourceChar(){
	int i=0;
	
	//if the status has value 0, then store the whole information in the line
	if(status == 0){
		while(true){
			c=fgetc(fp1);	
		   if (c == '\n') {
			   array[i] = '\0';//(MARK 1)store the information as '\0' instead of '\n' to avoid after printing the end of lines, the consor will go to next line		
			   break;
		   }
			if (c == EOF) {
				array[i] = EOF;
				break;
			}
			array[i] = c;
			i++;			
		}
		
	status = 1;//after make the array, set status to value 1, so that when call this function and get the character in the same line will not make the array again.
	fprintf(fp2,"%4d:%s\n",++line_num,array);	//print out the whole line into the output file
	}
	c = array[GetCurrentColumn()];//get the current character
	
	if (c!=EOF) {
			if(c == '\0'){//when c is a newline character, search for "MARK 1", for more infor mation
		        column_num = 0;				
				memset(array,0,MAXLINE+1);//clean the array
				status = 0;//set the status as value 0, since we have not stored information for the next line yet
				return '\n';
		}
		if(c!='\0'){
			column_num++;//if c is a charactor not at the end of the line, then just increase the column number
			return c;
		}
		}else {
			column_num = 0;//if c is the end of file then set the column value to 0, so that we can get catch the line in test file
			return EOF;
		}	
}

/* main of sendfile */
int main(int argc, char *argv[])
{
	OpenFiles(argv[0]);
	printf("Now this program is simply for testing the MD5 function.\n");
	if (argc < 2) {
		printf("<usage> %s [input_string]\n", argv[0]);
		exit(0);
	}

	uint8_t digest[16];
	int i;
	md5((uint8_t *) argv[1], strlen(argv[1]), digest);
	
	printf("MD5: ");
	for (i = 0; i < 16; i++)
		printf("%2.2x", digest[i]);
	printf("\n");

	return 0;
}
