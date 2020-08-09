

#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
kvarray_t * readKVs(const char * fname) {
  FILE * f;
  char * line = NULL;
  size_t sz = 0;
  kvpair_t * kvpair = NULL;
  kvpair = malloc(sizeof(*kvpair));
  kvarray_t * arr;
  size_t len = 0;
  if (fname == NULL) {
    fprintf(stderr, "there is no file");
    exit(EXIT_FAILURE);
  }
  f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "there is no file");
    exit(EXIT_FAILURE);
  }
  while ((getline(&line, &sz, f)) != -1) {
    kvpair = realloc(kvpair, (len + 1) * sizeof(*kvpair));
    if (kvpair == NULL) {
      exit(EXIT_FAILURE);
    }
    size_t lenvalue = 0;
    size_t lenkey = 0;
    if (strchr(line, '=') == NULL) {
      exit(EXIT_FAILURE);
    }
    (kvpair + len)->key = malloc(sizeof(*(kvpair + len)->key));
    (kvpair + len)->value = malloc(sizeof(*(kvpair + len)->value));
    while (line[lenkey] != '=') {
      // lenkey++;
      /* if (lenkey > sizeof(line)) {
        break;
        exit(EXIT_FAILURE);
	}*/

      // (kvpair + len)->key =
      //  realloc((kvpair + len)->key, (lenkey + 1) * sizeof(*(kvpair + len)->key));
      (kvpair + len)->key[lenkey] = line[lenkey];

      lenkey++;
      (kvpair + len)->key =
          realloc((kvpair + len)->key, (lenkey + 1) * sizeof(*(kvpair + len)->key));
    }
    (kvpair + len)->key[lenkey] = '\0';
    while ((line[lenkey + 1 + lenvalue] != '\0') &&
           (line[lenkey + 1 + lenvalue] != '\n')) {
      (kvpair + len)->value[lenvalue] = line[lenkey + 1 + lenvalue];

      lenvalue++;
      (kvpair + len)->value =
          realloc((kvpair + len)->value, (lenvalue + 1) * sizeof(*(kvpair + len)->value));
    }
    (kvpair + len)->value[lenvalue] = '\0';
    /*    (kvpair + len)->key = malloc((lenkey + 1) * sizeof(*(kvpair + len)->value));
    (kvpair + len)->value = malloc((lenvalue + 1) * sizeof(*(kvpair + len)->value));
    for (int i = 0; i < lenkey; i++) {
      (kvpair + len)->key[i] = line[i];
    }
    for (int i = 0; i < lenvalue; i++) {
      (kvpair + len)->value[i] = line[lenkey + 1 + i];
      }*/

    // free(line);
    // line = NULL;
    len++;
  }
  free(line);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    exit(EXIT_FAILURE);
  }
  arr = malloc(sizeof(*arr));
  if (arr == NULL) {
    exit(EXIT_FAILURE);
  }
  arr->kvpair = kvpair;
  arr->len = len;
  return arr;

  //WRITE ME
}

void freeKVs(kvarray_t * pairs) {
  kvpair_t * p;
  p = pairs->kvpair;
  for (int i = 0; i < pairs->len; i++) {
    free((p + i)->key);
    free((p + i)->value);
    // free(p + i);
  }
  /* for (int i = pairs->len - 1; i >= 0; i--) {
    free(p + i);
    }*/
  free(p);
  free(pairs);
  //WRITE ME
}

void printKVs(kvarray_t * pairs) {
  kvpair_t * p;
  p = pairs->kvpair;
  for (int i = 0; i < pairs->len; i++) {
    printf("key = '%s' value = '%s'\n", (p + i)->key, (p + i)->value);
  }
  // free(p);
  //WRITE ME
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  kvpair_t * p;
  int i;
  if (key == NULL) {
    perror("no key");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < pairs->len; i++) {
    p = &pairs->kvpair[i];
    if (strcmp(key, p->key) == 0) {
      return p->value;
    }
  }
  return NULL;
  //WRITE ME
}
