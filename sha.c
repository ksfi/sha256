#include "util.h"

// returnrs a 512 elements bin array
int *padding(char *str)
{
  size_t len = strlen(str);
  printf("taille %lu\n", len*8);
  int padded[64] = {0};
  int k = (448 - (len*8 + 1)) % 512;
  int *ret2 = malloc(sizeof(int)*(8*(len)+1+k+64)),
      *complement = malloc(sizeof(int)*16);

  /* "1" is appended.  Example: if the original message is
   * "01010000", this is padded to "010100001". 
   *  GOOD */
  memcpy(ret2, charToBin(str), sizeof(int)*len*8);
  ret2[len*8] = 1;

  /*  K "0"s are appended where K is the smallest, non-negative solution to the equation:
   *  L + 1 + K = 448 (mod 512) */
  int s = 0;
  for (int i = 8*(len)+1; i < 8*(len+1)+k; i++)
    ret2[i] = 0;

	/*    c.  Then append the 64-bit block that is L in binary representation.
        After appending this block, the length of the message will be a
        multiple of 512 bits. 
        GOOD
  */
  complement = bin(len, 16, complement);
  memcpy(padded+64-16, complement, sizeof(int)*16);
  memcpy(ret2+8*(len)+1+k, padded, sizeof(int)*64);
  return ret2;
} 

// 64 words of 32 bits
void blocks(int *binary_messagge, int W[64][32])
{
  int temp, temp1, temp2, temp3, temp4;

  // firsts 16
  for (int i = 0; i < 16; i++) {
    for (int j = 32*i, k = 0; k < 32; j++, k++) {
      W[i][k] = binary_messagge[j];
    }
  }

  // remaining 48
  for (int i = 16; i < 64; i++) {
    temp = SIGMA_1(toInt(W[i-2], 32));
    temp1 = toInt(W[i-7], 32);
    temp2 = SIGMA_0(toInt(W[i-15], 32));
    temp3 = toInt(W[i-16], 32);
    temp4 = addition_mod32(addition_mod32(temp, temp2), addition_mod32(temp2, temp3));

    for (int j = 0; j < 32; j++) {
      W[i][j] = bin(temp4, sizeof(toInt(W[0], 32))*8, W[i])[j]; 
    }
  }
}

void hash_computation(char *msg, int hash[8])
{
  int W[64][32], H[64][8];
  int a, b, c, d, e, f, g, h, t1, t2;
  int len = 1;

  blocks(padding(msg), W);

  H[0][0] = 0x6a09e667;
  H[0][1] = 0xbb67ae85; 
  H[0][2] = 0x3c6ef372;
  H[0][3] = 0xa54ff53a;
  H[0][4] = 0x510e527f;
  H[0][5] = 0x9b05688c;
  H[0][6] = 0x1f83d9ab;
  H[0][7] = 0x5be0cd19;

  for (int i = 1; i < len+1; i++) {
    printf("KODAZJLKZJELJAJ\n");
    a = H[i-1][0];
    b = H[i-1][1];
    c = H[i-1][2];
    d = H[i-1][3];
    e = H[i-1][4];
    f = H[i-1][5];
    g = H[i-1][6];
    h = H[i-1][7];
    for (int j = 0; j < 64; j++) {
      t1 = h+EPS_1(e)+CH(e, f, g)+binary_word[j]+toInt(W[j], 32);
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
    H[i][0] = H[i-1][0]+a;
    H[i][1] = H[i-1][1]+b;
    H[i][2] = H[i-1][2]+c;
    H[i][3] = H[i-1][3]+d;
    H[i][4] = H[i-1][4]+e;
    H[i][5] = H[i-1][5]+f;
    H[i][6] = H[i-1][6]+g;
    H[i][7] = H[i-1][7]+h;
  }
  for (int i = 0; i < 8; i++)
    hash[i] = H[63][i];
}


int main()
{
  char *msg = "tous les jours on boit de l'eau et pas d'alcool";
  int *array = malloc(sizeof(int)*4);
  int W[64][32], hash[8];
  hash_computation(msg, hash);
//   bin(hash[0], 32, arr);

/* ------------------------------ */

  for (int i = 0; i < 63; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%d", hash[i]);
    }
  }
  space();
  bar();

  int n = 0x5be0cd19;
  int *b = malloc(sizeof(int)*4);
  bin(n, 32, b);
  for (int i = 0; i < 32; i++)
    printf("%d", b[i]);
  space();
  bar();
  printf("ret %04x\n", toInt(b, 32));

//   printf("n %04x\n", n);

//   for (int i = 0; i < 64; i++) {
//     for (int j = 0; j < 32; j++) {
//       printf("%d", W[i][j]);
//     }
//     space();
//     if (i == 15)
//       bar();
//   }
}





