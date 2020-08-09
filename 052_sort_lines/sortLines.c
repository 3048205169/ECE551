#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void filesort(FILE * f) {
  char ** lines = NULL;
  char * curr = NULL;
  size_t size;
  size_t i = 0;
  while (getline(&curr, &size, f) >= 0) {
    lines = realloc(lines, (i + 1) * sizeof(*lines));
    lines[i] = curr;
    curr = NULL;
    i++;
  }
  free(curr);
  sortData(lines, i);
  for (size_t j = 0; j < i; j++) {
    fprintf(stdout, "%s", lines[j]);
    free(lines[j]);
  }
  free(lines);
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    filesort(stdin);
    return EXIT_SUCCESS;
  }
  else {
    size_t filenum = argc - 1;
    FILE ** F;
    F = malloc(filenum * sizeof(*F));
    for (size_t i = 0; i < filenum; i++) {
      F[i] = fopen(argv[i + 1], "r");
      if (F[i] == NULL) {
        fprintf(stderr, "Could not open file %lu", i + 1);

        free(F[i]);

        free(F);
        return EXIT_FAILURE;
      }
      filesort(F[i]);
      if (fclose(F[i]) != 0) {
        fprintf(stderr, "Could not close file %lu", i + 1);
        free(F[i]);
        free(F);
        return EXIT_FAILURE;
      }
      // free(F[i]);
    }
    free(F);
  }

  //WRITE YOUR CODE HERE!

  return EXIT_SUCCESS;
}
