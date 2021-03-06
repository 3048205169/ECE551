#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void getmatrix(FILE * f, char * Matrix) {
  int c;
  int i = 0;
  int j = 0;
  int flag = 0;
  while ((c = fgetc(f)) != EOF) {
    if (i > 10) {
      fprintf(stderr, "You can only enter ten lines\n");
      exit(EXIT_FAILURE);
      break;
    }
    else if (c == 10) {
      if (j != 10) {
        fprintf(stderr, "Every Line should have Ten Elements\n");
        exit(EXIT_FAILURE);
        break;
      }
      flag = 1;
      i++;
    }
    else if ((c < 32 || c > 126)) {
      fprintf(stderr, "Please Enter a printable char\n");
      exit(EXIT_FAILURE);
      break;
    }
    else if (j > 10) {
      fprintf(stderr, "Every Line should have Ten Elements\n");
      exit(EXIT_FAILURE);
      break;
    }
    else {
      if (j == 10) {
        if (flag == 1) {
          j = 0;
          flag = 0;
        }
        else {
          fprintf(stderr, "Every Line should have Ten Elements\n");
          exit(EXIT_FAILURE);
          break;
        }
      }
      if (i > 9 || j > 9) {
        fprintf(stderr, "You should enter a 10*10 matrix\n");
        exit(EXIT_FAILURE);
        break;
      }
      Matrix[i * 10 + j] = c;
      j++;
    }
  }
  if ((i != 10) && (j != 10)) {
    fprintf(stderr, "You should enter a 10*10 Matrix\n");
    exit(EXIT_FAILURE);
  }
}

void rotateMatrix(char * Matrix) {
  char matrix[10][10];
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      matrix[i][j] = Matrix[i * 10 + j];
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      Matrix[j * 10 + 9 - i] = matrix[i][j];
    }
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: only one inputfile\n");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  char Matrix[10][10] = {"a", "a", "a", "a", "a", "a", "a", "a", "a", "a"};
  getmatrix(f, &Matrix[0][0]);
  rotateMatrix(&Matrix[0][0]);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      fprintf(stdout, "%c", Matrix[i][j]);
    }
    fprintf(stdout, "\n");
  }

  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    exit(EXIT_FAILURE);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
