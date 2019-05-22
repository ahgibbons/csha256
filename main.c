#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sha256Funcs.h"
#include "sha256const.h"

// Message block is 512 bit, 16 x 32bit words
// Message block is 64 8-bit characters




struct WordPair {
	uint32_t x;
	uint32_t y;
};

size_t readfilechunks(char *mb, int chunksize, FILE *fp) {
	size_t a;
	int i;
	size_t len=0;

	do {
		a = fread(mb, 1, chunksize, fp);
		for (i=0;i<a;i++) {
			printf("%c", mb[i]);
		}
		printf("\n");
		len += a;
	} while (a != 0);
	return len;
};

unsigned int getfilesize(FILE *fp) {
	unsigned int filesize;
	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return filesize;
};


int main(int argc, char const *argv[])
{

	

	//char filename[25] = "testlonger.txt";
	char filename[25] = "test2.txt";
	char unsigned *messagebuffer2;
	unsigned int filesize;
	


	// Open file
	FILE *fp = fopen(filename, "r");
	filesize = getfilesize(fp); // length in bytes.
	messagebuffer2 = (char *) malloc(filesize);
	fread(messagebuffer2, 1, filesize, fp);
	fseek(fp, 0L, SEEK_SET);


	// Process chunk by chunk
	int chunksize;
	unsigned char *chunkbuffer = malloc(BLOCKSIZE_BYTE);
	chunksize = BLOCKSIZE_BYTE;
	size_t readval;
	int i;

	// Hashing Variables

	uint32_t a,b,c,d,e,f,g,h;
	uint32_t T1,T2;
	unsigned int messageIndex = 0;
	uint32_t W[16];

	uint32_t H[8];
	memcpy(H, H0, 32);


	unsigned int t;
	unsigned int messagelength=0;
	while ((readval=fread(chunkbuffer,1,chunksize,fp))>0) {
		messageIndex++;
		messagelength += readval;
		for (i=0;i<readval;i++) {
			printf("%c", chunkbuffer[i]);
		}
		// Initialise message schedule (W)
		// Stash smash!!!!
		for (t=0;t<MESSAGESCHEDULE;t++) {
			printf("t val%d\n", t);
			//W[t] = chunkbuffer[t];
		}
		printf("\nMessage Index: %u\n", messageIndex);
		if (readval < BLOCKSIZE_BYTE) {
			printf("Pad message\n");
			chunkbuffer = padmessage(chunkbuffer, readval, 
									messagelength);
		}


	}
	printf("Message length %d\n", messagelength);
	printf("Message length (bits) %d\n", messagelength*8);

	//Initial hash value

	// Clean up
	fclose(fp);
	free(messagebuffer2);

	return 0;
};