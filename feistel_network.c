#include <stdio.h>

int main(void) {
  unsigned long long P; // 64-bits
  printf("Enter the plain-text: ");
  scanf("%lld", &P);
  
  int K; // 32-bits
  printf("Enter the key: ");
  scanf("%d", &K);
  
  // 32-bit mask with all 1s
  int mask = ((unsigned long long)1 << 32) - 1;
  int R0 = P&mask; // to get the 2nd half of P
  int L0 = P>>32; // to get the 1st half of P
  
  int L1 = R0;
  int R1 = L0 ^ (R0 ^ K); // f = R0 ^ K
  
  unsigned long long C = L1;
  C = (C<<32) | R1;
  
  printf("The cipher-text is: %lld", C);
}