#include "retirement.h"

#include <stdio.h>
#include <stdlib.h>
/* function reads each line from a file between the specified dates,
   rorFromLine fcn ptr indicates how ror should be calculated 
   based on type of input data */
#define CTB 100;  //monthly contribuation
#define BALA 0;   //the first month balance
void error() {
  perror("something wrong");
  exit(EXIT_FAILURE);
}

double tenpow(int x) {
  double pow = 1;
  for (int i = 0; i < x; i++) {
    pow = pow * 10;
  }
  return pow;
}
double readnumber(char * line) {
  char * endptr;
  long int p1 = strtol(line, &endptr, 10);
  char * line2;
  double num;
  if (*endptr == '.') {
    line2 = endptr + 1;
    long int p2 = strtol(line2, &endptr, 10);
    int x = endptr - line2;
    num = (double)p1 + (double)p2 / tenpow(x);
  }
  else
    num = p1;
  return num;
}

int monthscalcul(double start, double end) {
  int syear = (int)start;
  int eyear = (int)end;
  int smonth = 101 * (start - (double)syear);
  int emonth = 101 * (end - (double)eyear);

  if (syear > eyear || ((syear == eyear) && (smonth > emonth))) {
    error();
  }
  //compute the months it spend
  int months = 12 * (eyear - syear - 1) + emonth + 12 - smonth + 1;
  return months;
}

ror_arr_t * readFile(FILE * f,
                     double start,
                     double end,
                     double (*rorFromLine)(char *, double *)) {
  if (f == NULL) {
    printf("there is no file");
    return NULL;
  }

  //compute the months it spend
  int months = monthscalcul(start, end);
  ror_arr_t * ROR = malloc(months * sizeof(*ROR));
  char * line = NULL;
  size_t linesz = 0;
  size_t i = 0;
  double datecheck = 0;
  double datestart = 0;  //find the date before the start date
  double * index = malloc((months + 1) * sizeof(*index));
  size_t j = 0;  //for index
  int syear = (int)start;
  int smonth = 101 * (start - (double)syear);
  if (smonth == 1) {
    datestart = start - 1;
  }
  else
    datestart = start - 0.01;
  while (getline(&line, &linesz, f) != -1) {
    if (!(line[0] == '1' || line[0] == '2')) {
      continue;
    }
    //record the data of the ROR
    char * endptr;
    long int p1 = strtol(line, &endptr, 10);
    char * line2 = endptr + 1;
    long int p2 = strtol(line2, &endptr, 10);
    datecheck = (double)p1 + (double)p2 / 100;
    if (datecheck == datestart) {
      line2 = endptr + 1;
      p1 = strtol(line2, &endptr, 10);
      line2 = endptr + 1;
      p2 = strtol(line2, &endptr, 10);
      index[j] = (double)p1 + (double)p2 / 100;
      j++;
    }
    if (datecheck >= start && datecheck <= end) {
      (ROR + i)->dates = datecheck;
      line2 = endptr + 1;

      //computer ROR
      if (j == 0) {
        (ROR + i)->ROR = rorFromLine(line2, NULL);
      }
      else
        (ROR + i)->ROR = rorFromLine(line2, &index[j - 1]);

      //computer index
      /*p1 = strtol(line2, &endptr, 10);
      if (*endptr == '.') {
        line2 = endptr + 1;
        p2 = strtol(line2, &endptr, 10);
        int x = endptr - line2;
        index[j] = (double)p1 + (double)p2 / tenpow(x);
      }
      else
      index[j] = p1;*/
      index[j] = readnumber(line2);
      i++;
      j++;
    }
  }
  free(index);
  free(line);

  return ROR;
}
//SP ror compute
double SProrFromLine(char * line, double * ROR_l) {
  if (ROR_l == NULL) {
    return 0;
  }

  //get index level and dividend from line
  char * endptr;
  long int p1 = strtol(line, &endptr, 10);
  char * line2;
  long int p2;
  double index;
  if (*endptr == '.') {
    line2 = endptr + 1;
    p2 = strtol(line2, &endptr, 10);
    int x = endptr - line2;
    index = (double)p1 + (double)p2 / tenpow(x);
  }
  else
    index = p1;
  line2 = endptr + 1;
  double divi;
  p1 = strtol(line2, &endptr, 10);
  if (*endptr == '.') {
    line2 = endptr + 1;
    p2 = strtol(line2, &endptr, 10);
    int x = endptr - line2;
    divi = (double)p1 + (double)p2 / tenpow(x);
  }
  else
    divi = p1;
  //  printf("%.2f,%.2f,%.2f\n", index, divi, *ROR_l);
  double ROR = ((index / (*ROR_l)) - (double)1) + (divi / (double)12) / index;
  return ROR;
}
//bond ror compute
double bondrorFromLine(char * line, double * ROR_l) {
  if (ROR_l == NULL) {
    return 0;
  }

  char * endptr;
  long int p1 = strtol(line, &endptr, 10);
  char * line2;
  double bond;
  if (*endptr == '.') {
    line2 = endptr + 1;
    long int p2 = strtol(line2, &endptr, 10);
    int x = endptr - line2;
    bond = (double)p1 + (double)p2 / tenpow(x);
  }
  else
    bond = p1;
  return bond / 1200;
}
//investment strategy 1 and 2
ret_portfolio_t * portcalcul12(ror_arr_t * ROR, int months) {
  ret_portfolio_t * p = malloc(sizeof(*p));
  p->dates = malloc(months * sizeof(*(p->dates)));
  p->cum_invest = malloc(months * sizeof(*(p->cum_invest)));
  p->n = months;
  p->cum_invest[0] = BALA;
  p->dates[0] = ROR->dates;
  double CT = CTB;
  for (int i = 1; i < months; i++) {
    p->dates[i] = (ROR + i)->dates;
    int year = (int)p->dates[i];
    int month = 101 * (p->dates[i] - (double)year);
    if (month == 1 && i != 1) {
      CT = (double)1.025 * CT;
    }

    p->cum_invest[i] = p->cum_invest[i - 1] * ((double)1 + (ROR + i)->ROR) + CT;
  }
  return p;
}

//investment strategy 3
ret_portfolio_t * portcalcul3(ror_arr_t * SPROR, ror_arr_t * BROR, int months) {
  ret_portfolio_t * p = malloc(sizeof(*p));
  p->dates = malloc(months * sizeof(*(p->dates)));
  p->cum_invest = malloc(months * sizeof(*(p->cum_invest)));
  p->n = months;
  p->cum_invest[0] = BALA;
  p->dates[0] = SPROR->dates;
  double life = 1;
  double CT = CTB;
  for (int i = 1; i < months; i++) {
    p->dates[i] = (SPROR + i)->dates;
    int year = (int)p->dates[i];
    int month = 101 * (p->dates[i] - (double)year);
    if (month == 1 && i != 1) {
      CT = (double)1.025 * CT;
      if (life > 0)
        life = life - 0.02;
    }

    p->cum_invest[i] = p->cum_invest[i - 1] * life * ((double)1 + (SPROR + i)->ROR) +
                       p->cum_invest[i - 1] * (1 - life) * ((double)1 + (BROR + i)->ROR) +
                       CT;
  }
  return p;
}

int main(int argc, char ** argv) {
  if (argc != 5) {
    fprintf(stderr, "Wrong Argument.\n");
    error();
  }
  FILE * fSP;
  FILE * fB;
  fSP = fopen(argv[1], "r");
  if (fSP == NULL) {
    fprintf(stderr, "open file SP fail");
    error();
  }
  fB = fopen(argv[2], "r");
  if (fB == NULL) {
    fprintf(stderr, "open file Bond fail");
    error();
  }
  double start = readnumber(argv[3]);
  double end = readnumber(argv[4]);
  int months = monthscalcul(start, end);
  if (start < 1970.01 || end > 2019.03) {
    fprintf(stderr, "Enter the wrong date.\n");
    error();
  }

  ror_arr_t * SP;
  ret_portfolio_t * SPP;
  SP = readFile(fSP, start, end, SProrFromLine);
  SPP = portcalcul12(SP, months);

  ror_arr_t * bond;
  ret_portfolio_t * bondP;
  bond = readFile(fB, start, end, bondrorFromLine);
  bondP = portcalcul12(bond, months);

  ret_portfolio_t * s3;
  s3 = portcalcul3(SP, bond, months);

  FILE * fw = fopen("portfolio.csv", "w");
  if (fw == NULL) {
    fprintf(stderr, "open file SP fail");
    error();
  }
  for (int i = 1; i < months; i++) {
    fprintf(fw,
            "%.2f,%.2f,%.2f,%.2f\n",
            SPP->dates[i],
            SPP->cum_invest[i],
            bondP->cum_invest[i],
            s3->cum_invest[i]);
  }

  if ((fclose(fw) != 0) || (fclose(fSP) != 0) || (fclose(fB) != 0)) {
    fprintf(stderr, "close file fail\n");
    error();
  }
  free(SP);
  free(SPP->dates);
  free(SPP->cum_invest);
  free(SPP);

  free(bond);
  free(bondP->dates);
  free(bondP->cum_invest);
  free(bondP);
  free(s3->cum_invest);
  free(s3->dates);
  free(s3);
  return EXIT_SUCCESS;
}
