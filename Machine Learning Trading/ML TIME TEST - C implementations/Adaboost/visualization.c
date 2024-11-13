// visualization.c
#include "adaboost.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 100
#define HEIGHT 30
#define TEST_SIZE 100

// ANSI color codes
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define BOLD "\x1b[1m"
#define BG_GREEN "\x1b[42m"
#define BG_RED "\x1b[41m"

typedef struct {
  float price;
  float open;
  float high;
  float low;
  float volume;
  float change;
  int predicted;
  int actual;
} DataPoint;

// Function to normalize value between min and max
int normalize(float value, float min, float max, int height) {
  return (int)((value - min) * (height - 1) / (max - min));
}

void visualize_predictions() {
  // Open and read data
  FILE *file = fopen("NSE All Share Historical Data Cleaned.csv", "r");
  if (!file) {
    printf("Error opening file for visualization\n");
    return;
  }

  // Skip header
  char line[1024];
  fgets(line, sizeof(line), file);

  // Store data points
  DataPoint *data = malloc(TEST_SIZE * sizeof(DataPoint));
  int count = 0;
  float min_price = 999999, max_price = 0;

  // Read data
  while (fgets(line, sizeof(line), file) && count < TEST_SIZE) {
    char *token = strtok(line, ",");
    int field = 0;

    while (token != NULL) {
      switch (field) {
      case 1: // Price
        data[count].price = atof(token);
        min_price =
            data[count].price < min_price ? data[count].price : min_price;
        max_price =
            data[count].price > max_price ? data[count].price : max_price;
        break;
      case 2: // Open
        data[count].open = atof(token);
        break;
      case 3: // High
        data[count].high = atof(token);
        break;
      case 4: // Low
        data[count].low = atof(token);
        break;
      case 5: // Volume
        data[count].volume = atof(token);
        break;
      case 6: // Change %
        data[count].change = atof(token);
        data[count].actual = (atof(token) > 0) ? 1 : -1;
        break;
      }
      token = strtok(NULL, ",");
      field++;
    }

    // Get prediction
    data[count].predicted =
        stockAdaboost(data[count].price, data[count].open, data[count].high,
                      data[count].low, data[count].volume);

    count++;
  }
  fclose(file);

  // Print header
  printf("\n%s%sStock Price Prediction Visualization%s\n", BOLD, YELLOW, RESET);
  printf("%sPrice range: %.2f - %.2f%s\n\n", BLUE, min_price, max_price, RESET);

  // Create visualization grid
  char grid[HEIGHT][WIDTH];
  memset(grid, ' ', HEIGHT * WIDTH * sizeof(char));

  // Plot price line and predictions
  for (int i = 0; i < count - 1 && i < WIDTH - 1; i++) {
    int y1 = normalize(data[i].price, min_price, max_price, HEIGHT);
    int y2 = normalize(data[i + 1].price, min_price, max_price, HEIGHT);

    // Draw line between points
    int start_y = y1 < y2 ? y1 : y2;
    int end_y = y1 < y2 ? y2 : y1;

    for (int y = start_y; y <= end_y; y++) {
      grid[HEIGHT - 1 - y][i] = '|';
    }

    // Mark predictions
    char marker = ' ';
    if (data[i].predicted == data[i].actual) {
      marker = data[i].predicted == 1 ? '↑' : '↓';
    } else {
      marker = 'x';
    }
    grid[HEIGHT - 1 - y1][i] = marker;
  }

  // Print visualization
  printf("%s Price ┌", BOLD);
  for (int i = 0; i < WIDTH; i++)
    printf("─");
  printf("┐%s\n", RESET);

  for (int y = 0; y < HEIGHT; y++) {
    printf("%7.0f │", min_price + (max_price - min_price) * (HEIGHT - 1 - y) /
                                      (HEIGHT - 1));
    for (int x = 0; x < WIDTH; x++) {
      char c = grid[y][x];
      if (c == '↑')
        printf("%s%c%s", GREEN, c, RESET);
      else if (c == '↓')
        printf("%s%c%s", RED, c, RESET);
      else if (c == 'x')
        printf("%s%c%s", YELLOW, c, RESET);
      else if (c == '|')
        printf("%s%c%s", BLUE, c, RESET);
      else
        printf("%c", c);
    }
    printf("│\n");
  }

  printf("       └");
  for (int i = 0; i < WIDTH; i++)
    printf("─");
  printf("┘\n");

  // Print legend
  printf("\n%sLegend:%s\n", BOLD, RESET);
  printf("%s↑%s : Correct Upward Prediction\n", GREEN, RESET);
  printf("%s↓%s : Correct Downward Prediction\n", RED, RESET);
  printf("%sx%s : Incorrect Prediction\n", YELLOW, RESET);
  printf("%s|%s : Price Movement\n\n", BLUE, RESET);

  // Print statistics
  int correct = 0;
  int total = 0;
  for (int i = 0; i < count; i++) {
    if (data[i].predicted == data[i].actual)
      correct++;
    total++;
  }

  printf("%sStatistics:%s\n", BOLD, RESET);
  printf("Total predictions: %d\n", total);
  printf("Correct predictions: %d\n", correct);
  printf("Accuracy: %.2f%%\n\n", (float)correct / total * 100);

  free(data);
}

int main() {
  loadTrainingData("NSE All Share Historical Data Cleaned.csv");
  visualize_predictions();
  return 0;
}
