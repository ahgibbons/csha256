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

	// Load file
	FILE *fp;

	char filename[25] = "testlonger.txt";
	char messagebuffer[BLOCKSIZE_BYTE];
	char unsigned *messagebuffer2;
	unsigned int filesize;

	fp = fopen(filename, "r");
	filesize = getfilesize(fp); // length in bytes.
	messagebuffer2 = (char *) malloc(filesize);
	fread(messagebuffer2, 1, filesize, fp);
	fseek(fp, 0L, SEEK_SET);

	// Pad message
	messagebuffer2 = padmessage(messagebuffer2, filesize);
	if (messagebuffer2==NULL) {
		return MEMORYERROR;
	}

	//Initial hash value
	unsigned char H[64];
	memcpy(H, H0, 64);
	charprint(H, 64);
	

	// Clean up
	fclose(fp);
	free(messagebuffer2);

	return 0;
};