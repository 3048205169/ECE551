#include <stdio.h>

size_t maxSeq(int * array, size_t n) {
  if (n == 0) {
    return 0;
  }
  size_t num[n];
  size_t j = 0;
  num[j] = 1;
  for (size_t i = 0; i < (n - 1); i++) {
    if (*(array + i) < *(array + i + 1)) {
      num[j]++;
    }
    else {
      j++;
      num[j] = 1;
    }
  }
  size_t final = 1;
  for (size_t i = 0; i <= j; i++) {
    if (final < num[i]) {
      final = num[i];
    }
  }
  return final;
}
