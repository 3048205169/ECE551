#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int maxposition(int * array, int num) {
  int max = 0;
  for (int i = 0; i < num; i++) {
    if (array[max] < array[i])
      max = i;
  }
  return max;
}

int breakkey(FILE * f) {
  int c;
  int frequency[26] = {0};
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      c -= 'a';
      frequency[c]++;
    }
  }
  int e = 0;
  e = maxposition(frequency, 26);
  int key = e - 4;
  key = key % 26;
  if (key < 0) {
    key = key + 26;
  }
  return key;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: break filename\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Could not open file\n");
    return EXIT_FAILURE;
  }
  int key;
  key = breakkey(f);
  fprintf(stdout, "%d", key);
  if (fclose(f) != 0) {
    fprintf(stderr, "Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
