#include <stdio.h>

#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  int ans = low;
  while (low < high) {
    int mid = low + (high - low) / 2;
    int value = f->invoke(mid);
    if (value <= 0)
      low = mid + 1;
    else
      high = mid;
    //   printf("mid is %d\n", mid);
  }
  if (ans == low)
    return low;
  return low - 1;
}
