// adaboost.c
#include "adaboost.h" // Added this include
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SAMPLES 1000
#define ITERATION 3

static struct StockData trainingData[MAX_SAMPLES]; // Made static
static int numSamples = 0;

// Weak classifiers
static int priceThresholdClassifier(float price, float threshold) {
  return (price > threshold) ? 1 : -1;
}

static int volumeThresholdClassifier(float volume, float threshold) {
  return (volume > threshold) ? 1 : -1;
}

static int volatilityClassifier(float high, float low, float threshold) {
  float volatility = (high - low) / low;
  return (volatility > threshold) ? 1 : -1;
}

void loadTrainingData(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error opening file: %s\n", filename);
    return;
  }

  char line[1024];
  // Skip header
  fgets(line, sizeof(line), file);

  while (fgets(line, sizeof(line), file) && numSamples < MAX_SAMPLES) {
    char *token = strtok(line, ",");
    int field = 0;

    while (token != NULL) {
      switch (field) {
      case 1: // Price
        trainingData[numSamples].price = atof(token);
        break;
      case 2: // Open
        trainingData[numSamples].open = atof(token);
        break;
      case 3: // High
        trainingData[numSamples].high = atof(token);
        break;
      case 4: // Low
        trainingData[numSamples].low = atof(token);
        break;
      case 5: // Volume
        trainingData[numSamples].volume = atof(token);
        break;
      case 6: // Change %
        trainingData[numSamples].change = atof(token);
        // Set trend based on price change
        trainingData[numSamples].trend = (atof(token) > 0) ? 1 : -1;
        break;
      }
      token = strtok(NULL, ",");
      field++;
    }
    numSamples++;
  }

  fclose(file);
  printf("Loaded %d samples from %s\n", numSamples, filename);
}

int stockAdaboost(float price, float open, float high, float low,
                  float volume) {
  (void)open; // Suppress unused parameter warning

  if (numSamples == 0) {
    printf("No training data loaded!\n");
    return 0;
  }

  float *weights = (float *)calloc(numSamples, sizeof(float));
  float alpha[ITERATION] = {0};
  int *predictions = (int *)calloc(numSamples, sizeof(int));

  // Initialize weights
  for (int i = 0; i < numSamples; i++) {
    weights[i] = 1.0f / numSamples;
  }

  // AdaBoost iterations
  for (int t = 0; t < ITERATION; t++) {
    float minError = INFINITY;
    int bestClassifier = 0;
    float bestThreshold = 0;

    // Try different thresholds for each classifier
    float priceThresholds[] = {price * 0.95f, price, price * 1.05f};
    float volumeThresholds[] = {volume * 0.5f, volume, volume * 1.5f};
    float volatilityThresholds[] = {0.01f, 0.02f, 0.03f};

    // Find best classifier and threshold
    for (int c = 0; c < 3; c++) {
      float *thresholds;
      int numThresholds;

      switch (c) {
      case 0:
        thresholds = priceThresholds;
        numThresholds = sizeof(priceThresholds) / sizeof(float);
        break;
      case 1:
        thresholds = volumeThresholds;
        numThresholds = sizeof(volumeThresholds) / sizeof(float);
        break;
      case 2:
        thresholds = volatilityThresholds;
        numThresholds = sizeof(volatilityThresholds) / sizeof(float);
        break;
      }

      for (int th = 0; th < numThresholds; th++) {
        float error = 0;

        for (int i = 0; i < numSamples; i++) {
          int prediction;
          switch (c) {
          case 0:
            prediction =
                priceThresholdClassifier(trainingData[i].price, thresholds[th]);
            break;
          case 1:
            prediction = volumeThresholdClassifier(trainingData[i].volume,
                                                   thresholds[th]);
            break;
          case 2:
            prediction = volatilityClassifier(
                trainingData[i].high, trainingData[i].low, thresholds[th]);
            break;
          }

          if (prediction != trainingData[i].trend) {
            error += weights[i];
          }
        }

        if (error < minError) {
          minError = error;
          bestClassifier = c;
          bestThreshold = thresholds[th];
        }
      }
    }

    // Calculate alpha
    alpha[t] =
        0.5f * log((1 - minError) /
                   (minError +
                    1e-10)); // Added small constant to prevent division by zero

    // Update weights and store predictions
    float weightSum = 0;
    for (int i = 0; i < numSamples; i++) {
      int prediction;
      switch (bestClassifier) {
      case 0:
        prediction =
            priceThresholdClassifier(trainingData[i].price, bestThreshold);
        break;
      case 1:
        prediction =
            volumeThresholdClassifier(trainingData[i].volume, bestThreshold);
        break;
      case 2:
        prediction = volatilityClassifier(trainingData[i].high,
                                          trainingData[i].low, bestThreshold);
        break;
      }

      predictions[i] = prediction;
      weights[i] *= exp(-alpha[t] * prediction * trainingData[i].trend);
      weightSum += weights[i];
    }

    // Normalize weights
    for (int i = 0; i < numSamples; i++) {
      weights[i] /= weightSum;
    }
  }

  // Make final prediction
  float finalPrediction = 0;
  for (int t = 0; t < ITERATION; t++) {
    int prediction;
    switch (t % 3) {
    case 0:
      prediction = priceThresholdClassifier(price, price);
      break;
    case 1:
      prediction = volumeThresholdClassifier(volume, volume);
      break;
    case 2:
      prediction = volatilityClassifier(high, low, 0.02f);
      break;
    }
    finalPrediction += alpha[t] * prediction;
  }

  free(weights);
  free(predictions);

  return (finalPrediction >= 0) ? 1 : -1;
}
