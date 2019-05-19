#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sha256Funcs.h"
#include "sha256const.h"

// Message block is 512 bit, 16 x 32bit words
// Message block is 64 8-bit characters

// TODO read input from file

#define BLOCKSIZE_BIT 512
#define BLOCKSIZE_BYTE 64 // Number of bytes in each chunk
#define WORDSIZE 32 // w = 32
#define BLOCKNUM 16 // words
#define ZEROPADNUM 448 // number of pad zeros ]
#define ONEPAD 0x80 // b10000000
#define MEMORYERROR -1

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

void charprint(char *mbuf, int n) {
	for (int i=0;i<n;i++) {
		printf("%c", mbuf[i]);
	}
	printf("\n");
};

unsigned int mod(unsigned int a, unsigned int d) {
	return a % d;
}


unsigned char *padmessage(unsigned char *mbuf, unsigned int mlen) {
	unsigned long bitsize = mlen * 8; // bytes to bits
	unsigned int nzerobits = mod(ZEROPADNUM - bitsize -1, BLOCKSIZE_BIT);
	unsigned int nzerobytes = (nzerobits-7) / 8;

	unsigned int newsize = ((bitsize+1)/BLOCKSIZE_BIT+1) * BLOCKSIZE_BIT;
	unsigned int newsize_bytes = newsize/8;
	printf("New size (bits): %u\n",newsize);
	printf("New size (bytes): %u\n",newsize/8);


	printf("Padding -> %lu + 1 + %u + 64\n", bitsize, nzerobits);
	printf("%u byte(s) + onebyte + %u zero bytes + 8 bytes\n", 
		mlen, nzerobytes);


	mbuf = realloc(mbuf, sizeof(char)*newsize_bytes);
	if (mbuf== NULL) {
		return mbuf;
	}

	int lentrack = mlen;

	unsigned char onepad = ONEPAD;
	mbuf[mlen] = onepad;
	lentrack++;

	int zero_track = 0;
	int nzeros;
	unsigned int mpos=mlen+1; // Add zeros
	for (nzeros=nzerobytes; nzeros>0; nzeros--) {
		mbuf[mpos] = 0;
		mpos++;
		lentrack++;
		zero_track++;
	}

	printf("pos after zeros %u\n", mpos);

	int rshifter;
	for (rshifter=7;rshifter>=0;rshifter--) {
		unsigned char a = (bitsize >> (8*rshifter)) & 0xff;
		mbuf[mpos] = a;
		printf("mpos: %u\n", mpos);
		printf("len byte %u\n", a);
		printf("rshifter %d\n", rshifter);
		mpos++;
		lentrack++;
	}

	printf("zero track: %d\n", zero_track);
	printf("lentrack: %d\n", lentrack);
	printf("mpos: %u\n", mpos);
	charprint(mbuf, lentrack);

	printf("char 24: %u\n", mbuf[lentrack-1]);

	return mbuf;
};


int main(int argc, char const *argv[])
{

	FILE *fp;

	char filename[25] = "testlonger.txt";
	char messagebuffer[BLOCKSIZE_BYTE];
	char unsigned *messagebuffer2;
	size_t mlength;
	unsigned int filesize;

	fp = fopen(filename, "r");
	filesize = getfilesize(fp); // length in bytes.

	int mbitsize = filesize*8;


	messagebuffer2 = (char *) malloc(filesize);
	fread(messagebuffer2, 1, filesize, fp);


	printf("%s\n\n", messagebuffer2);

	printf("File bitsize %d\n", filesize*8);
	printf("Old size (bytes): %u\n", filesize);
	messagebuffer2 = padmessage(messagebuffer2, filesize);


	fseek(fp, 0L, SEEK_SET);

	printf("filesize: %d\n", filesize);

	fclose(fp);
	free(messagebuffer2);

	return 0;
};