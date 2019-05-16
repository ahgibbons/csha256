#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sha256Funcs.h"
#include "sha256const.h"

// Message block is 512 bit, 16 x 32bit words
// Message block is 64 8-bit characters

#define BLOCKSIZE_BIT = 512
#define WORDSIZE 32 // w = 32
#define BLOCKNUM 16 // words

struct WordPair {
	uint32_t x;
	uint32_t y;
};

int main(int argc, char const *argv[])
{
	uint32_t a,b,c;

	int message_length = 3;
	int bitlength = 3*8;

	char *message;
	message = (char *) malloc(4);
	strcpy(message, "abc");

	

	int bitblock_size = ((bitlength + 1) / 512) * 512 + 512;
	int padamount = bitblock_size - (bitlength+1);


	printf("Message Size (bit) %d\n", block_size_bit);
	printf("%d\n", padamount);

	return 0;
};