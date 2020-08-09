
#include "counts.h"

#include <stdlib.h>
#include <string.h>

#include "stdio.h"

counts_t * createCounts(void) {
  counts_t * c;
  //printf("1111");
  c = malloc(sizeof(*c));
  if (c == NULL) {
    exit(EXIT_FAILURE);
  }
  c->len = 0;
  c->onecount = NULL;
  c->unknow = 0;
  // printf("111");
  return c;
  //WRITE ME
}
void addCount(counts_t * c, const char * name) {
  if (name == NULL) {
    // exit(EXIT_FAILURE);
    c->unknow++;
    return;
  }
  // printf("111");
  size_t i = 0;
  for (; i < c->len; i++) {
    if (strcmp((&c->onecount[i])->value, name) == 0)
      break;
  }
  if (i < c->len) {
    (&c->onecount[i])->count++;
  }
  else {
    c->len++;
    c->onecount = realloc(c->onecount, c->len * sizeof(*(c->onecount)));
    if (c->onecount == NULL) {
      exit(EXIT_FAILURE);
    }
    // (&c->onecount[c->len])->value = malloc(sizeof(*name));
    (&c->onecount[c->len - 1])->value = strdup(name);
    if ((&c->onecount[c->len - 1])->value == NULL) {
      exit(EXIT_FAILURE);
    }
    // strcpy((&c->onecount[c->len])->value, name);
    (&c->onecount[c->len - 1])->count = 1;
  }

  //WRITE ME
}
void printCounts(counts_t * c, FILE * outFile) {
  for (size_t i = 0; i < c->len; i++) {
    fprintf(outFile, "%s: %d\n", (&c->onecount[i])->value, (&c->onecount[i])->count);
  }
  // printf("111");
  if (c->unknow > 0)
    fprintf(outFile, "<unknown> : %d\n", c->unknow);

  //WRITE ME
}

void freeCounts(counts_t * c) {
  for (size_t i = 0; i < c->len; i++) {
    free((&c->onecount[i])->value);
  }
  free(c->onecount);
  free(c);
  //WRITE ME
}
