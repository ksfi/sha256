#ifndef SHA_HEADER
#define SHA_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

#include "sha.h"

#define BLOCK_SIZE 512

#define SET_BIT(x, y) x |= (1 << y)
#define LEFT_CIRC_SHIFT(x, n) (((x) << (n)) | ((x) >> (sizeof(x) * 8 - (n))))
#define RIGHT_CIRC_SHIFT(x, n) (((x) >> (n)) | ((x) << (sizeof(x) * 8 - (n))))
#define CH(x, y, z) ((x&y)^(~(x)&z))
#define MAJ(x, y, z) ((x&y)^(x&z)^(y&z))
#define EPS_0(x) (RIGHT_CIRC_SHIFT(x, 2)^RIGHT_CIRC_SHIFT(x,13)^RIGHT_CIRC_SHIFT(x, 22))
#define EPS_1(x) (RIGHT_CIRC_SHIFT(x, 6)^RIGHT_CIRC_SHIFT(x, 11)^RIGHT_CIRC_SHIFT(x, 25))
#define SIGMA_0(x) (RIGHT_CIRC_SHIFT(x, 7)^RIGHT_CIRC_SHIFT(x, 18)^(x >> 3))
#define SIGMA_1(x) (RIGHT_CIRC_SHIFT(x, 17)^RIGHT_CIRC_SHIFT(x, 19)^(x >> 10))

struct ctx {
  // len: number of elements in binary
  size_t len, 
         n_bocks;
  int *binary,
      **binary_parsed;
};

struct ctx *init(size_t len)
{
  struct ctx *ret = malloc(sizeof(struct ctx));
  ret->binary = malloc(sizeof(int)*len);
  ret->len = len*8;
  ret->n_bocks = len/512;
  ret->binary_parsed = malloc(sizeof(int)*ret->n_bocks);
  return ret;
  free(ret);
}

unsigned int binary_word[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

void space()
{
  printf("\n");
}

void bar()
{
  printf("------------\n");
}

int addition_mod32(int a, int b)
{
  return ((a+b)%4294967296);
} 

void *initiateArray(size_t len, size_t len_element)
{
  int **ret = malloc(sizeof(int)*len);
  for (int i = 0; i < len; i++)
    ret[i] = malloc(sizeof(int) * len_element);
  return ret;
}

void printArray(int *arr, size_t len)
{
//   len = sizeof(int) * len;
  for (size_t i = 0; i < len; i++)
    printf("%d", arr[i]);
}

// unsigned -> len bits bin representation
// size_t len: length in bits ex: int=32, char=8
int *bin(unsigned n, size_t len, int *arr) 
{
  unsigned i = 1 << (len - 1);
  for(unsigned j = 0; i > 0; i = i >> 1, j++) {
    arr[j] = (n & i) != 0;
  }
  return arr;
}

// len bits representation -> unsigned
// size_t len: length in bits ex: int=32, char=8
int toInt(int *bin_representation, size_t len)
{
  unsigned ret = 0;
  for (int i = 0; i < len; i++)
    ret = ret + bin_representation[i]*pow(2, len-i-1);
  return ret;
}

int *charToBin(char *str)
{
  size_t len = strlen(str);
  int *ret = malloc(sizeof(int)*len*8),
      *temp = malloc(sizeof(int)*8);
  for (int i = 0; i < len; i++) {
    temp = bin(str[i], 8, temp);
    for (int j = 0; j < 8; j++)
      ret[8*i+j] = temp[j];
  }
  free(temp);
  return ret;
}

#endif
