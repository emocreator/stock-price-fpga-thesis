// adaboost.h
#ifndef ADABOOST_H
#define ADABOOST_H

struct StockData {
  float price;
  float open;
  float high;
  float low;
  float volume;
  float change;
  int trend; // 1 if price went up, -1 if down
};

int stockAdaboost(float price, float open, float high, float low, float volume);
void loadTrainingData(const char *filename);

#endif
