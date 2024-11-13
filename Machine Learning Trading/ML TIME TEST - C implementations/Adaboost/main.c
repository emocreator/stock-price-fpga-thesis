// main.c
#include "adaboost.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Added this header for strtok

#define TEST_SIZE 100 // Number of points to use for testing

int main() {
  // Load training data
  loadTrainingData("NSE All Share Historical Data Cleaned.csv");

  // Open the file again to get test data
  FILE *file = fopen("NSE All Share Historical Data Cleaned.csv", "r");
  if (!file) {
    printf("Error opening file for testing\n");
    return 1;
  }

  // Skip header
  char line[1024];
  fgets(line, sizeof(line), file);

  // Arrays to store test data
  float *prices = malloc(TEST_SIZE * sizeof(float));
  float *opens = malloc(TEST_SIZE * sizeof(float));
  float *highs = malloc(TEST_SIZE * sizeof(float));
  float *lows = malloc(TEST_SIZE * sizeof(float));
  float *volumes = malloc(TEST_SIZE * sizeof(float));
  float *actual_changes = malloc(TEST_SIZE * sizeof(float));

  int count = 0;

  // Read test data
  while (fgets(line, sizeof(line), file) && count < TEST_SIZE) {
    char *token = strtok(line, ",");
    int field = 0;

    while (token != NULL) {
      switch (field) {
      case 1: // Price
        prices[count] = atof(token);
        break;
      case 2: // Open
        opens[count] = atof(token);
        break;
      case 3: // High
        highs[count] = atof(token);
        break;
      case 4: // Low
        lows[count] = atof(token);
        break;
      case 5: // Volume
        volumes[count] = atof(token);
        break;
      case 6: // Change %
        actual_changes[count] = atof(token);
        break;
      }
      token = strtok(NULL, ",");
      field++;
    }
    count++;
  }
  fclose(file);

  // Make predictions and calculate accuracy
  int correct_predictions = 0;
  int true_positives = 0;
  int false_positives = 0;
  int true_negatives = 0;
  int false_negatives = 0;

  printf("\nMaking predictions for %d test points:\n", count);
  printf("----------------------------------------\n");
  printf("Point   Actual    Predicted   Correct?\n");
  printf("----------------------------------------\n");

  for (int i = 0; i < count; i++) {
    int prediction =
        stockAdaboost(prices[i], opens[i], highs[i], lows[i], volumes[i]);

    int actual_trend = (actual_changes[i] > 0) ? 1 : -1;
    int is_correct = (prediction == actual_trend);

    if (is_correct) {
      correct_predictions++;
      if (prediction == 1)
        true_positives++;
      else
        true_negatives++;
    } else {
      if (prediction == 1)
        false_positives++;
      else
        false_negatives++;
    }

    printf("%4d %8s %10s      %s\n", i + 1,
           actual_changes[i] > 0 ? "UP" : "DOWN",
           prediction == 1 ? "UP" : "DOWN", is_correct ? "✓" : "✗");

    if ((i + 1) % 20 == 0) { // Print separator every 20 lines
      printf("----------------------------------------\n");
    }
  }

  // Calculate metrics
  float accuracy = (float)correct_predictions / count * 100;
  float precision =
      (float)true_positives / (true_positives + false_positives) * 100;
  float recall =
      (float)true_positives / (true_positives + false_negatives) * 100;
  float f1_score = 2 * (precision * recall) / (precision + recall);

  // Display results
  printf("\nPerformance Metrics:\n");
  printf("----------------------------------------\n");
  printf("Total predictions: %d\n", count);
  printf("Correct predictions: %d\n", correct_predictions);
  printf("Accuracy: %.2f%%\n", accuracy);
  printf("Precision: %.2f%%\n", precision);
  printf("Recall: %.2f%%\n", recall);
  printf("F1 Score: %.2f\n", f1_score);
  printf("\nDetailed Counts:\n");
  printf("True Positives: %d\n", true_positives);
  printf("False Positives: %d\n", false_positives);
  printf("True Negatives: %d\n", true_negatives);
  printf("False Negatives: %d\n", false_negatives);

  // Clean up
  free(prices);
  free(opens);
  free(highs);
  free(lows);
  free(volumes);
  free(actual_changes);

  return 0;
}
