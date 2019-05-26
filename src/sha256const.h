// sha256const.h
#ifndef SHA256CONST
#define SHA256CONST

#define BLOCKSIZE_BIT 512
#define BLOCKSIZE_BYTE 64 // Number of bytes in each chunk
#define WORDSIZE 32 // w = 32
#define BLOCKNUM 16 // words
#define MEMORYERROR -1
#define MESSAGESCHEDULE 64

#include <stdint.h>

const uint32_t H0[8];
const uint32_t K[64];

#endif
