#ifndef __STATS_H__
#define __STATS_H__

#include <stdlib.h>

#define MAX_NAME_LEN 64

struct stat_tag {
  char player[MAX_NAME_LEN];
  unsigned long points;
  unsigned games;
};
typedef struct stat_tag stat_t;

stat_t parseLine(const char * line);

double calcGamePerformance(stat_t * statarr, unsigned long * pointarr, size_t nplayers);

void addGameStats(stat_t * statarr, unsigned long * pointarr, size_t nplayers);

void printBreakout(stat_t * statarr, unsigned long * pointarr, size_t nplayers);

#endif
