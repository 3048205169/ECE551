#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};
void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int ans;
  ans = binarySearchForZero(f, low, high);
  //  printf("%d\n", ans);
  if (ans != expected_ans) {
    fprintf(stderr, "Too many function invocations in %s\n", mesg);
    exit(EXIT_FAILURE);
  }
}

class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

void totalcheck(int low,
                int high,
                int expected,
                unsigned iter,
                const char * msg1,
                const char * msg2) {
  SinFunction * sin = new SinFunction();
  CountedIntFn * fun = new CountedIntFn(iter, sin, msg1);
  check(fun, low, high, expected, msg2);
  delete sin;
  delete fun;
}

int main() {
  const char * msg1 = "iteration";
  const char * msg2 = "expected";
  //SinFunction * sin = new SinFunction();
  totalcheck(1, 1, 1, 1, msg1, msg2);
  totalcheck(0, 150000, 52359, 18, msg1, msg2);
  totalcheck(-2, 0, -1, 2, msg1, msg2);
  totalcheck(1, 2, 1, 1, msg1, msg2);
  totalcheck(-1, 3, 2, 3, msg1, msg2);
  totalcheck(100000, 250000, 100000, 18, msg1, msg2);
  totalcheck(0, 2, 1, 2, msg1, msg2);
  totalcheck(-150000, 150000, 52359, 19, msg1, msg2);
  totalcheck(-2, 13, 12, 4, msg1, msg2);
  totalcheck(-78700, 183431, 52359, 19, msg1, msg2);
}
