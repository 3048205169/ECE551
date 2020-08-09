#include "stats.h"

#include "stdio.h"
// any other headers you need here

unsigned long powm(double x, int y) {
  unsigned long product = 1;
  for (int i = 0; i < y; i++) {
    product = product * x;
  }
  return product;
}

stat_t parseLine(const char * line) {
  int count = 0;
  stat_t statdata;
  statdata.points = 0;
  statdata.games = 0;
  for (int i = 0; i < 64; i++) {
    statdata.player[i] = '\0';
  }

  if (line[0] == ':') {
    fprintf(stderr, "Wrong Syntax");
    exit(EXIT_FAILURE);
  }

  while (line[count] != ':') {
    if (count == 64 ||
        !((line[count] == 32) || (line[count] == ',') || (line[count] == '.') ||
          ((line[count] >= 'a') && (line[count] <= 'z')) || ((line[count] == 39)) ||
          ((line[count] >= 'A') && (line[count] <= 'Z')))) {
      fprintf(stderr, "Please enter normal character in name.");
      exit(EXIT_FAILURE);
    }
    statdata.player[count] = line[count];
    count++;
  }

  int countnumber = 0;
  int number[10];
  count++;
  if (line[count] == '0' && line[count + 1] != ':') {
    fprintf(stderr, "the first digitals of number should not be zero.");
    exit(EXIT_FAILURE);
  }  // test if the first digitals is 0

  while (line[count] != ':') {
    if (countnumber >= 10 || !((line[count] >= '0') && (line[count] <= '9'))) {
      fprintf(stderr, "Please enter number");
      exit(EXIT_FAILURE);
    }
    number[countnumber] = line[count] - '0';
    countnumber++;
    count++;
  }
  if (countnumber == 0) {
    fprintf(stderr, "there is no number");
    exit(EXIT_FAILURE);
  }  //test if there is no number

  //test if the size of the numer over the size of unsigned long
  unsigned long long testnumber = 0;
  for (int i = 0; i < countnumber; i++) {
    testnumber += number[countnumber - 1 - i] * powm(10, i);
  }
  if (testnumber > 4294967295) {
    fprintf(stderr, "the size of number is too large");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < countnumber; i++) {
    statdata.points += number[countnumber - 1 - i] * powm(10, i);
  }

  //get games
  count++;

  //test if the first digial is 0
  if (line[count] == '0' && line[count + 1] != ':') {
    fprintf(stderr, "the first digitals of number should not be zero.");
    exit(EXIT_FAILURE);
  }

  //reinitial number
  for (int i = 0; i < 10; i++) {
    number[i] = 0;
  }
  countnumber = 0;

  //get number
  while (!(line[count] == '\n' || line[count] == '\0')) {
    if ((countnumber >= 10) || !((line[count] >= '0') && (line[count] <= '9'))) {
      fprintf(stderr, "please enter number");
      exit(EXIT_FAILURE);
    }

    number[countnumber] = line[count] - '0';
    countnumber++;
    count++;
  }

  //if there is no number
  if (countnumber == 0) {
    fprintf(stderr, "there is no number.");
    exit(EXIT_FAILURE);
  }
  testnumber = 0;
  //test if the size of games over the size of unsigned int
  for (int i = 0; i < countnumber; i++) {
    testnumber += number[countnumber - 1 - i] * powm(10, i);
  }
  if (testnumber > 4294967295) {
    fprintf(stderr, "the size of number is too large.");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < countnumber; i++) {
    statdata.games += number[countnumber - 1 - i] * powm(10, i);
  }

  //printf

  return statdata;

  // STEP 1: write me
}

double calcGamePerformance(stat_t * statarr, unsigned long * pointarr, size_t nplayers) {
  double sumpast = 0;
  double sumnow = 0;
  if (statarr == NULL || pointarr == NULL) {
    fprintf(stderr, "No data");
    exit(EXIT_FAILURE);
  }
  if (nplayers == 0) {
    return 0;
  }

  // int avgpoint[nplayers];
  for (size_t i = 0; i < nplayers; i++) {
    sumpast += (double)statarr[i].points / statarr[i].games;
  }
  for (size_t i = 0; i < nplayers; i++) {
    sumnow += pointarr[i];
  }
  //  if (sumpast == 0) {
  // printf("They get no points past\n");
  // return sumnow / sumpast;
  // }
  // printf("%f\n", sumpast);
  // printf("%f\n", sumnow);
  //  double frac = sumnow / sumpast;
  return sumnow / sumpast;

  // STEP 2: write me
}

void addGameStats(stat_t * statarr, unsigned long * pointarr, size_t nplayers) {
  if (nplayers == 0 || statarr == NULL || pointarr == NULL) {
    fprintf(stderr, "No data");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < nplayers; i++) {
    // printf("%s have %u games and get %lu points before\n",
    //     statarr[i].player,
    //     statarr[i].games,
    //     statarr[i].points);
    statarr[i].games++;
    statarr[i].points += pointarr[i];
    // printf("%s have %u games and get %lu points after\n",
    //      statarr[i].player,
    //       statarr[i].games,
    //      statarr[i].points);
  }

  // STEP 3: write me
}

void printBreakout(stat_t * statarr, unsigned long * pointarr, size_t nplayers) {
  if (nplayers == 0 || statarr == NULL || pointarr == NULL) {
    printf("No data");
    exit(EXIT_FAILURE);
  }
  double avg[nplayers];
  double breakfrac[nplayers];
  for (size_t i = 0; i < nplayers; i++) {
    avg[i] = (double)statarr[i].points / statarr[i].games;
    breakfrac[i] = (double)pointarr[i] / avg[i];
    // printf("The breakout player this game is %s with %.0f%% of agv ppg\n",
    //     statarr[i].player,
    //     100 * breakfrac[i]);
  }

  double maxfrac = 0;
  int position = 0;
  for (size_t i = 0; i < nplayers; i++) {
    if (breakfrac[i] > maxfrac) {
      position = i;
      maxfrac = breakfrac[i];
    }
  }
  printf("The breakout player this game is %s with %.0f%% of agv ppg\n",
         statarr[position].player,
         100 * maxfrac);

  // STEP 4: write me
}
