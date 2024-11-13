// visualize.c
#include "adaboost.h"
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define ST_MIN 0
#define ST_MAX 10
#define DST_MIN 0
#define DST_MAX 3

// ANSI color codes
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"
#define BOLD "\x1b[1m"

void visualize_boundary() {
  char grid[HEIGHT][WIDTH];
  float st_step = (ST_MAX - ST_MIN) / (float)WIDTH;
  float dst_step = (DST_MAX - DST_MIN) / (float)HEIGHT;

  // Generate decision boundary
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      float st = ST_MIN + x * st_step;
      float dst = DST_MAX - y * dst_step; // Invert Y axis

      int result = f1Adaboost(st, dst);
      grid[y][x] = (result == 1) ? '+' : '-';
    }
  }

  // Print the visualization with colors
  printf("\n%s%sDecision Boundary Visualization:%s\n", BOLD, YELLOW, RESET);
  printf("%s(X axis: Sleep Time, Y axis: Deep Sleep Time)%s\n\n", BLUE, RESET);

  for (int y = 0; y < HEIGHT; y++) {
    printf("%s%4.1f%s |", BOLD, DST_MAX - y * dst_step, RESET);
    for (int x = 0; x < WIDTH; x++) {
      if (grid[y][x] == '+') {
        printf("%s%c%s", GREEN, grid[y][x], RESET);
      } else {
        printf("%s%c%s", RED, grid[y][x], RESET);
      }
    }
    printf("|\n");
  }

  // Print X axis
  printf("     +");
  for (int x = 0; x < WIDTH; x++)
    printf("-");
  printf("+\n");

  printf("    %.1f", (float)ST_MIN);
  for (int x = 1; x < 8; x++) {
    printf("%10.1f", ST_MIN + x * (ST_MAX - ST_MIN) / 8.0);
  }
  printf("\n\n");

  // Print legend
  printf("%sLegend:%s\n", BOLD, RESET);
  printf("%s+%s : Class 1 (Normal Sleep Pattern)\n", GREEN, RESET);
  printf("%s-%s : Class -1 (Abnormal Sleep Pattern)\n\n", RED, RESET);
}

int main() {
  visualize_boundary();
  return 0;
}
