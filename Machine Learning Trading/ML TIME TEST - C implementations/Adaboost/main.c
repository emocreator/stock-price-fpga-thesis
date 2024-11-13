// main.c
#include "adaboost.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  struct test {
    float st;
    float dst;
  } testData[14] = {{5, 0.5},   {5.5, 0.9}, {3, 1.1},   {2.5, 1.4}, {5.5, 1.2},
                    {5.6, 1.6}, {6.1, 1.1}, {7.5, 1.5}, {5.7, 1.6}, {7, 2},
                    {6, 0.9},   {8, 1.15},  {25, 4},    {8, 3}};

  struct test *p = testData;
  int result;

  // Test Adaboost classifier
  for (int i = 0; i < 14; i++) {
    float st = (p + i)->st;
    float dst = (p + i)->dst;
    result = f1Adaboost(st, dst);
    printf("Result for test point %d: %d\n", i + 1, result);
  }

  return 0;
}
