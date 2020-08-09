#include <stdio.h>
#include <stdlib.h>

int seq2(int x) {
  return (x - 2) * (x - 2) - 2;
}

int sum_seq2(int low, int high) {
  if (low >= high) {
    return 0;
  }
  else {
    int sum = 0;
    for (int i = low; i < high; i++) {
      sum = sum + seq2(i);
    }
    return sum;
  }
}

int main(void) {
  for (int i = -20; i < 20; i++) {
    printf("seq2(%d) = %d\n", i, seq2(i));
  }
  printf("seq2(%d) = %d\n", 10000, seq2(10000));
  printf("seq2(%d) = %d\n", -10000, seq2(-10000));
  printf("seq2(%d) = %d\n", 12349, seq2(12349));

  printf("sum_seq2(%d, %d) = %d\n", 0, 2, sum_seq2(0, 2));
  printf("sum_seq2(%d, %d) = %d\n", 3, 6, sum_seq2(3, 6));
  printf("sum_seq2(%d, %d) = %d\n", 9, 7, sum_seq2(9, 7));
  printf("sum_seq2(%d, %d) = %d\n", 0, 0, sum_seq2(0, 0));
  for (int i = -3; i < 13; i++) {
    for (int j = -3; j < 13; j++) {
      printf("sum_seq2(%d, %d) = %d\n", i, j, sum_seq2(i, j));
    }
  }
  printf("sum_seq2(%d, %d) = %d\n", 10000, 10010, sum_seq2(10000, 10010));
  printf("sum_seq2(%d, %d) = %d\n", -10000, -9990, sum_seq2(-10000, -9990));
  return EXIT_SUCCESS;
}
//This file is for Step 2.
//You should do
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sum_seq2
//  Step 2 (D): add test cases to main to test sum_seq2
//
// Be sure to #include any header files you need!
