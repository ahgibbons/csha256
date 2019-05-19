// sha256Funcs.h
#ifndef SHA256FUNCS_H
#define SHA256FUNCS_H

#define ZEROPADNUM 448 // number of pad zeros ]
#define ONEPAD 0x80 // b10000000

#include <stdint.h>

uint32_t madd(uint32_t a, uint32_t b); 
uint32_t rotr(uint32_t x, int n);
uint32_t rotl(uint32_t x, int n);
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t LSigma0(uint32_t x);
uint32_t LSigma1(uint32_t x);
uint32_t sigma0(uint32_t x);
uint32_t sigma1(uint32_t x);
unsigned char *padmessage(unsigned char *mbuf, unsigned int mlen);
unsigned int mod(unsigned int a, unsigned int d);
void charprint(char *mbuf, int n);

#endif