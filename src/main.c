#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "sha256Funcs.h"
#include "sha256const.h"


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
	for (t=16;t<MESSAGESCHEDULE;t++) {
		W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];
	}

	// Run through message schedule
	for (t=0;t<MESSAGESCHEDULE;t++) {
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

	// Update message digest
	H[0] += a;
	H[1] += b;
	H[2] += c;
	H[3] += d;
	H[4] += e;
	H[5] += f;
	H[6] += g;
	H[7] += h;

}

uint32_t *hashMessage(FILE *fp, uint32_t *H) {
	unsigned char bytebuffer[BLOCKSIZE_BYTE];
	uint32_t wordbuffer[BLOCKNUM]; 

	uint64_t messagelength=0;
	size_t readval;


	while ((readval=fread(bytebuffer,1,
		BLOCKSIZE_BYTE,fp))==BLOCKSIZE_BYTE) {
		messagelength += readval;

		// Convert to uint32 words
		bytesToWords(wordbuffer, bytebuffer);

		hashRound(wordbuffer, H);
	}

	// process final chunk of message
	messagelength += readval;
	padmessage(bytebuffer, readval, messagelength);
	bytesToWords(wordbuffer, bytebuffer);
	hashRound(wordbuffer, H);

	return H;
}


int main(int argc, char const *argv[])
{
	unsigned int filesize;
	FILE *fp;

	// Open file
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Error, Could not open file: %s\n", argv[1]);
		return 1;
	}

	// Initial Hash Value
	uint32_t digest[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19};
	hashMessage(fp, digest);
	fclose(fp);


	printf("%08x%08x%08x%08x%08x%08x%08x%08x  %s\n", 
		digest[0], digest[1], digest[2], digest[3], digest[4],
		digest[5], digest[6], digest[7], argv[1]);

	return 0;
};