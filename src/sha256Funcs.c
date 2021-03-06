// sha256Funcs.c

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

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
	return (x>>n) | (x<<(WORDSIZE-n));
};

uint32_t rotl(uint32_t x, int n) {
	return (x<<n) | (x>>(WORDSIZE-n));
};

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ ((~x) & z);
};

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (x & z) ^ (y & z);
};

uint32_t LSigma0(uint32_t x) {
	return (rotr(x,2) ^ rotr(x,13) ^ rotr(x,22));
};

uint32_t LSigma1(uint32_t x) {
	return (rotr(x,6) ^ rotr(x,11) ^ rotr(x,25));
};

uint32_t sigma0(uint32_t x) {
	return (rotr(x,7) ^ rotr(x,18) ^ (x >> 3));
};

uint32_t sigma1(uint32_t x) {
	return (rotr(x,17) ^ rotr(x,19) ^ (x >> 10));
};

unsigned int mod(uint64_t a, unsigned int d) {
	return a % d;
};

void bytesToWords(uint32_t *wordbuffer, unsigned char *bytebuffer) {
	for (int i=0;i<BLOCKNUM;i++) {
			wordbuffer[i] = (uint32_t) bytebuffer[4*i] << (3*8);
			wordbuffer[i] += (uint32_t) bytebuffer[4*i+1] << (2*8);
			wordbuffer[i] += (uint32_t) bytebuffer[4*i+2] << (1*8);
			wordbuffer[i] += (uint32_t) bytebuffer[4*i+3];
		}
}


void padmessage(unsigned char *mbuf,
						  unsigned int chsize,
						  uint64_t totallength) {
	uint64_t bitsize = (totallength * 8); // bytes to bits
	unsigned int nzerobits = mod(ZEROPADNUM - bitsize -1, BLOCKSIZE_BIT);
	unsigned int nzerobytes = (nzerobits-7) / 8;


	int lentrack = chsize;

	unsigned char onepad = ONEPAD;
	mbuf[chsize] = onepad;
	lentrack++;

	int nzeros;
	unsigned int mpos=chsize+1; // Add zeros
	for (nzeros=nzerobytes; nzeros>0; nzeros--) {
		mbuf[mpos] = 0;
		mpos++;
		lentrack++;
	}


	// Add 8 byte bit length of message
	unsigned long rshifter;
	for (rshifter=0;rshifter<8;rshifter++) {
		unsigned char a = (bitsize >> (8*rshifter)) & 0xff;
		mbuf[BLOCKSIZE_BYTE-1-rshifter] = a;

	}
};