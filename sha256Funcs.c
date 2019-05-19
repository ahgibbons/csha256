// sha256Funcs.c

#include <stdint.h>

#include "sha256Funcs.h"

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