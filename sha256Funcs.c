// sha256Funcs.c

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sha256Funcs.h"
#include "sha256const.h"

#define WORDSIZE 32
// 32 bit modulo addition
uint32_t madd(uint32_t a, uint32_t b) {
	uint32_t c;
	c = a + b;
	return c;
};

uint32_t rotr(uint32_t x, int n) {
	return (x>>n) ^ (x<<(WORDSIZE-n));
};

uint32_t rotl(uint32_t x, int n) {
	return (x<<n) ^ (x>>(WORDSIZE-n));
};

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (x ^ z);
};

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (x & z) ^ (y & z);
};

uint32_t LSigma0(uint32_t x) {
	return (rotr(x,2) ^ rotr(x,13) ^ rotr(x,22));
};

uint32_t LSigma1(uint32_t x) {
	return (rotr(x,6) ^ rotr(x,13) ^ rotr(x,22));
};

uint32_t sigma0(uint32_t x) {
	return (rotr(x,7) ^ rotr(x,18) ^ x >> 3);
};

uint32_t sigma1(uint32_t x) {
	return (rotr(x,17) ^ rotr(x,19) ^ x >> 10);
};

unsigned int mod(unsigned int a, unsigned int d) {
	return a % d;
};

void charprint(char *mbuf, int n) {
	for (int i=0;i<n;i++) {
		printf("%c", mbuf[i]);
	}
	printf("\n");
};


unsigned char *padmessage(unsigned char *mbuf, unsigned int mlen) {
	unsigned long bitsize = mlen * 8; // bytes to bits
	unsigned int nzerobits = mod(ZEROPADNUM - bitsize -1, BLOCKSIZE_BIT);
	unsigned int nzerobytes = (nzerobits-7) / 8;

	unsigned int newsize = ((bitsize+1)/BLOCKSIZE_BIT+1) * BLOCKSIZE_BIT;
	unsigned int newsize_bytes = newsize/8;



	mbuf = realloc(mbuf, sizeof(char)*newsize_bytes);
	if (mbuf== NULL) {
		return mbuf;
	}

	int lentrack = mlen;

	unsigned char onepad = ONEPAD;
	mbuf[mlen] = onepad;
	lentrack++;

	int nzeros;
	unsigned int mpos=mlen+1; // Add zeros
	for (nzeros=nzerobytes; nzeros>0; nzeros--) {
		mbuf[mpos] = 0;
		mpos++;
		lentrack++;
	}


	int rshifter;
	for (rshifter=7;rshifter>=0;rshifter--) {
		unsigned char a = (bitsize >> (8*rshifter)) & 0xff;
		mbuf[mpos] = a;

		mpos++;
		lentrack++;
	}

	charprint(mbuf, lentrack);

	printf("char 24: %u\n", mbuf[lentrack-1]);

	return mbuf;
};