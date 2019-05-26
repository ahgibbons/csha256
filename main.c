#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sha256Funcs.h"
#include "sha256const.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

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

unsigned int getfilesize(FILE *fp) {
	unsigned int filesize;
	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return filesize;
};

uint32_t *hashMessage(FILE *fp) {
	unsigned char bytebuffer[BLOCKSIZE_BYTE];
	uint32_t wordbuffer[BLOCKNUM]; 

	uint32_t a,b,c,d,e,f,g,h;
	uint32_t T1,T2;
	unsigned int messageIndex = 0;
	uint32_t W[64];


	uint32_t *H;
	H = malloc(32);
	memcpy(H, H0, 32);


	unsigned int t;
	unsigned int i;
	unsigned int messagelength=0;
	size_t readval;

	printf("Hash0:\n");
	for (i=0;i<8;i++) {
		printf("%08x\n", H[i]);
	}

	while ((readval=fread(bytebuffer,1,BLOCKSIZE_BYTE,fp))>0) {
		messageIndex++;
		messagelength += readval;
		for (int i=0;i<readval;i++) {
			printf("%c", bytebuffer[i]);
		}


		// Pad Message chunk if necessary
		printf("\nMessage Index: %u\n", messageIndex);
		if (readval < BLOCKSIZE_BYTE) {
			padmessage(bytebuffer, readval, 
						messagelength);
		}

		// Convert to uint32 words
		bytesToWords(wordbuffer, bytebuffer);
		for (i=0;i<4;i++) {
			printf("%08x ", (uint32_t) bytebuffer[i] << ((3-i)*8));
		}
		printf("\n");
		printf("%08x\n", wordbuffer[0]);


		printf("\nPadded Message:\n");
		for (i=0;i<64;i++) {
			printf("%d\t" BYTE_TO_BINARY_PATTERN "\n", 
				i, BYTE_TO_BINARY(bytebuffer[i]));
		}

		printf("\n");
		for (i=0;i<BLOCKNUM;i++) {
			printf("%08x\n", wordbuffer[i]);
		}
		printf("\n");


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

		printf("Message Schedule:\n");
		for (int i=0; i<64; i++) {
			printf("%d\t%08x\n", i, W[i]);
		}

		printf("Old a %08x\n", a);

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
			printf("t: %u\t T1: %08x T2: %08x a: %08x b: %08x c: %08x d: %08x e: %08x f: %08x g: %08x h: %08x\n",
					t, T1, T2, a,b,c,d,e,f,g,h);
		}

		printf("New a %08x\n", a);


		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;


	}
	printf("Message length %d\n", messagelength);
	printf("Message length (bits) %d\n", messagelength*8);

	return H;
}


int main(int argc, char const *argv[])
{
	char filename[25] = "test2.txt";
	unsigned int filesize;
	printf("%s\n", argv[1]);


	// Open file
	FILE *fp = fopen(argv[1], "r");
	filesize = getfilesize(fp); // length in bytes.

	uint32_t *digest;
	digest = hashMessage(fp);

	for (int i=0; i<8; i++) {
		printf("%08x\n", digest[i]);
	}

	// Clean up
	fclose(fp);
	free(digest);

	return 0;
};