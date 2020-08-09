#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  FILE * f;
  counts_t * c;
  char * line = NULL;
  size_t sz;
  int linesz;
  f = fopen(filename, "r");
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }
  c = createCounts();
  while ((linesz = getline(&line, &sz, f)) != -1) {
    if (linesz > 0 && line[linesz - 1] == '\n') {
      line[linesz - 1] = '\0';
    }
    addCount(c, lookupValue(kvPairs, line));
  }
  if (!feof(f)) {
    fprintf(stderr, "invalid input format\n");
    exit(EXIT_FAILURE);
  }
  free(line);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    exit(EXIT_FAILURE);
  }

  //WRITE ME
  return c;
}

int main(int argc, char ** argv) {
  if (argc <= 2) {
    exit(EXIT_FAILURE);
  }
  kvarray_t * arr;
  counts_t * c;
  arr = readKVs(argv[1]);
  printKVs(arr);
  for (int i = 2; i < argc; i++) {
    FILE * f;
    char * outputname;
    outputname = computeOutputFileName(argv[i]);
    c = countFile(argv[i], arr);
    f = fopen(outputname, "w");
    if (f == NULL) {
      exit(EXIT_FAILURE);
    }
    printCounts(c, f);
    if (fclose(f) != 0) {
      perror("Failed to close the input file!");
      exit(EXIT_FAILURE);
    }
    free(outputname);
    freeCounts(c);
  }
  freeKVs(arr);
  return EXIT_SUCCESS;
  //WRITE ME (plus add appropriate error checking!)
  //read the key/value pairs from the file named by argv[1] (call the result kv)

  //count from 2 to argc (call the number you count i)

  //count the values that appear in the file named by argv[i], using kv as the key/value pair
  //   (call this result c)

  //compute the output file name from argv[i] (call this outName)

  //open the file named by outName (call that f)

  //print the counts from c into the FILE f

  //close f

  //free the memory for outName and c

  //free the memory for kv

  return EXIT_SUCCESS;
}
