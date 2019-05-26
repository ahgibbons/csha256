#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sha256Funcs.h"
#include "sha256const.h"


// Message block is 512 bit, 16 x 32bit words
// Message block is 64 8-bit characters

// uint32_t is little endian in memory.


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

void hashRound(uint32_t *wordbuffer, uint32_t *H) {


	uint32_t a,b,c,d,e,f,g,h;
	uint32_t T1,T2;
	uint32_t W[64];
	unsigned int t;
	
	a = H[0];
	b = H[1];
	c = H[2];
	d = H[3];
	e = H[4];
	f = H[5];
	g = H[6];
	h = H[7];

	// Make message schedule
	memcpy(W, wordbuffer, BLOCKSIZE_BYTE);
	for (t=16;t<64;t++) {
		W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];
	}


	for (t=0;t<64;t++) {
		T1 = h + LSigma1(e) + Ch(e,f,g) + K[t] + W[t];
		T2 = LSigma0(a) + Maj(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1+T2;

	}


	H[0] += a;
	H[1] += b;
	H[2] += c;
	H[3] += d;
	H[4] += e;
	H[5] += f;
	H[6] += g;
	H[7] += h;

}

uint32_t *hashMessage(FILE *fp) {
	unsigned char bytebuffer[BLOCKSIZE_BYTE];
	uint32_t wordbuffer[BLOCKNUM]; 

	uint32_t *H;
	H = malloc(32);
	memcpy(H, H0, 32);

	unsigned int messagelength=0;
	size_t readval;


	while ((readval=fread(bytebuffer,1,
		BLOCKSIZE_BYTE,fp))==BLOCKSIZE_BYTE) {
		messagelength += readval;


		// Pad Message chunk if necessary
		if (readval < BLOCKSIZE_BYTE) {
			padmessage(bytebuffer, readval, 
						messagelength);
		}

		// Convert to uint32 words
		bytesToWords(wordbuffer, bytebuffer);

		hashRound(wordbuffer, H);


	}

	messagelength += readval;
	padmessage(bytebuffer, readval, messagelength);
	bytesToWords(wordbuffer, bytebuffer);
	hashRound(wordbuffer, H);

	return H;
}


int main(int argc, char const *argv[])
{
	unsigned int filesize;


	// Open file
	FILE *fp = fopen(argv[1], "r");

	uint32_t *digest;
	digest = hashMessage(fp);


	printf("%08x%08x%08x%08x%08x%08x%08x%08x  %s\n", 
		digest[0], digest[1], digest[2], digest[3], digest[4],
		digest[5], digest[6], digest[7], argv[1]);


	// Clean up
	fclose(fp);
	free(digest);

	return 0;
};