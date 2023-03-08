#include "util.h"

/* REWRITE ALL THIS WITH CTX STRUCT */

// returnrs a 512 elements bin array
// int *padding(char *str)
struct ctx *padding(char *str) /* DONE */
{
  size_t len = strlen(str);
  int k = (448 - (len*8 + 1)) % 512, 
      s = 0,
      padded[64] = {0}, 
      *complement = malloc(sizeof(int)*16);
  struct ctx *ret = init((8*(len)+1+k+64));

  /* "1" is appended.  Example: if the original message is
   * "01010000", this is padded to "010100001". 
   *  GOOD */
  memcpy(ret->binary, charToBin(str), sizeof(int)*len*8);
  ret->binary[len*8] = 1;

  /*  K "0"s are appended where K is the smallest, non-negative solution to the equation:
   *  L + 1 + K = 448 (mod 512) */
  for (int i = 8*(len)+1; i < 8*(len+1)+k; i++)
    ret->binary[i] = 0;

	/*    c.  Then append the 64-bit block that is L in binary representation.
        After appending this block, the length of the message will be a
        multiple of 512 bits. 
        GOOD
  */
  complement = bin(len, 16, complement);
  memcpy(padded+64-16, complement, sizeof(int)*16);
  memcpy(ret->binary+8*(len)+1+k, padded, sizeof(int)*64);
  return ret;
} 

void parse(struct ctx *ctx) /* DONE */
{
  int k = 0, l = 0;
  for (int i = 0; i < ctx->len; i++, l++) {
    if (i%512 == 0) {
      l = 0;
      k = i / 512;
      ctx->binary_parsed[k] = malloc(sizeof(int)*512);
    }
    ctx->binary_parsed[k][l] = ctx->binary[i];
  }
}

// 64 words of 32 bits
// void blocks(int *binary_messagge, int W[64][32])
void blocks(struct ctx *ctx, int W[ctx->n_bocks][64][32]) /* DONE */
{
  for (int b = 0; b < ctx->n_bocks; b++) {
    int temp, temp1, temp2, temp3, temp4;

    // firsts 16
    for (int i = 0; i < 16; i++) {
      for (int j = 32*i, k = 0; k < 32; j++, k++)
        W[b][i][k] = ctx->binary_parsed[b][j];
    }

    // remaining 48
    for (int i = 16; i < 64; i++) {
      temp = SIGMA_1(toInt(W[b][i-2], 32));
      temp1 = toInt(W[b][i-7], 32);
      temp2 = SIGMA_0(toInt(W[b][i-15], 32));
      temp3 = toInt(W[b][i-16], 32);
      temp4 = addition_mod32(addition_mod32(temp, temp2), addition_mod32(temp2, temp3));
      for (int j = 0; j < 32; j++)
        W[b][i][j] = bin(temp4, sizeof(toInt(W[b][0], 32))*8, W[b][i])[j]; 
    }
  }
}

void hash_computation(char *msg, int hash[8])
{
  struct ctx *t = padding(msg);

  int W[t->n_bocks][64][32], 
      H[t->n_bocks][8],
      a, b, c, d, e, f, g, h, t1, t2;

  parse(t);
  blocks(t, W);

  H[-1][0] = 0x6a09e667;
  H[-1][1] = 0xbb67ae85; 
  H[-1][2] = 0x3c6ef372;
  H[-1][3] = 0xa54ff53a;
  H[-1][4] = 0x510e527f;
  H[-1][5] = 0x9b05688c;
  H[-1][6] = 0x1f83d9ab;
  H[-1][7] = 0x5be0cd19;

  for (int k = 0; k < t->n_bocks; k++) {
    a = H[k-1][0];
    b = H[k-1][1];
    c = H[k-1][2];
    d = H[k-1][3];
    e = H[k-1][4];
    f = H[k-1][5];
    g = H[k-1][6];
    h = H[k-1][7];
    for (int j = 0; j < 64; j++) {
      t1 = h+EPS_1(e)+CH(e, f, g)+binary_word[j]+toInt(W[k][j], 32);
      t2 = SIGMA_0(a)+MAJ(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d+t1;
      d = c;
      c = b;
      b = a;
      a = t1+t2;
    }
    H[k][0] = H[k-1][0]+a;
    H[k][1] = H[k-1][1]+b;
    H[k][2] = H[k-1][2]+c;
    H[k][3] = H[k-1][3]+d;
    H[k][4] = H[k-1][4]+e;
    H[k][5] = H[k-1][5]+f;
    H[k][6] = H[k-1][6]+g;
    H[k][7] = H[k-1][7]+h;
  }

  for (int i = 0; i < 8; i++)
    hash[i] = H[t->n_bocks-1][i];
}


int main(int argv, char *argc[])
{
  printf("%s\n", argc[1]);
//   struct ctx *padding(char *str) /* DONE */
//   void parse(struct ctx *ctx)
//   void blocks(struct ctx *ctx, int W[ctx->n_bocks][64][32])
//   void hash_computation(char *msg, int hash[8])
  char *msg = "j'arrive tout de suite";
/* ------------------------------ */
  if (!argc[1]) return 0;

  int h[8];
  hash_computation(argc[1], h);
  for (int i = 0; i < 8; i++)
    printf("%08x", h[i]);
}





