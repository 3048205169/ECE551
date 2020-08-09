#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  size_t len;
  char * outputname;
  len = strlen(inputName);
  if (inputName == NULL) {
    exit(EXIT_FAILURE);
  }
  outputname = malloc((len + 8) * sizeof(*outputname));
  strncpy(outputname, inputName, len);
  strncpy(outputname + len, ".counts", 8);
  return outputname;
  //WRITE ME
}
