// f1Adaboost.c
#include <math.h>
#include <stdio.h>

#define DATANUM 10
#define ITERATION 3

// Function prototypes
static int sign(float fx);
static int H1(float x, float TH);
static int H2(float x, float TH);
static int CompClassierDiff(int iInitialDataVal, int iClassierVal);
static int CountErrRightNum(const int Data[], int Num);
static int CompMinErrValue(float err1, float err2);
static void WeightsUpdate(float fNewWcs[], const float fOldWcs[], float fErr,
                          const int ErrHx[], int ErrNumX, int DataNum);
static float CalcNewClassifersErr(const float fOldDataWeigths[],
                                  const int ErrHx[], int ErrNumX);

// Training data structure
struct TRAINING {
  float ST;
  float DST;
  int SLEEP;
};

int f1Adaboost(float st, float dst) {
  // Initialize arrays
  float fInitialDataWeigths[DATANUM] = {0};
  float fOldDataWeigths[DATANUM] = {0};
  float fNewDataWeigths[DATANUM] = {0};

  int ResultH1[DATANUM] = {0};
  int ResultH2[DATANUM] = {0};
  int ErrH1[DATANUM] = {0};
  int ErrH2[DATANUM] = {0};
  int RightH1[DATANUM] = {0};
  int RightH2[DATANUM] = {0};

  int iErrNum1 = 0, iErrNum2 = 0;
  int iRightNum1 = 0, iRightNum2 = 0;
  float fErr1 = 0, fErr2 = 0;
  float a1 = 0, a2 = 0, ax = 0;

  // Training data
  struct TRAINING TrainingData[DATANUM] = {
      {5, 0.2, 1}, {3, 1.6, 1},  {5, 0.6, 1},    {7, 0.8, 1},    {6.2, 0.6, 1},
      {7, 2, -1},  {8, 2.5, -1}, {6.2, 1.1, -1}, {7.5, 1.6, -1}, {7, 1.1, -1}};

  // Initialize weights
  for (int i = 0; i < DATANUM; i++) {
    fInitialDataWeigths[i] = 1.0f / DATANUM;
    fOldDataWeigths[i] = fInitialDataWeigths[i];
    fNewDataWeigths[i] = fInitialDataWeigths[i];
  }

  // Calculate initial classifier results
  for (int i = 0; i < DATANUM; i++) {
    ResultH1[i] = H1(TrainingData[i].ST, 6);
    ResultH2[i] = H2(TrainingData[i].DST, 1);
  }

  // Find errors and correct classifications
  for (int i = 0; i < DATANUM; i++) {
    int resultH1 = CompClassierDiff(TrainingData[i].SLEEP, ResultH1[i]);
    int resultH2 = CompClassierDiff(TrainingData[i].SLEEP, ResultH2[i]);

    if (resultH1 == 0) {
      RightH1[iRightNum1++] = (i + 1);
    } else {
      ErrH1[iErrNum1++] = (i + 1);
    }

    if (resultH2 == 0) {
      RightH2[iRightNum2++] = (i + 1);
    } else {
      ErrH2[iErrNum2++] = (i + 1);
    }
  }

  // Main AdaBoost iterations
  int ResultFinalL1 = 0, ResultFinalL2 = 0;

  for (int iter = ITERATION - 1; iter >= 0; iter--) {
    fErr1 = CalcNewClassifersErr(fOldDataWeigths, ErrH1, iErrNum1);
    fErr2 = CalcNewClassifersErr(fOldDataWeigths, ErrH2, iErrNum2);

    int iMinErrNum = CompMinErrValue(fErr1, fErr2);

    if (iMinErrNum == 1) {
      ax = 0.5f * log((1 - fErr1) / fErr1);
      WeightsUpdate(fNewDataWeigths, fOldDataWeigths, fErr1, ErrH1, iErrNum1,
                    DATANUM);
    } else {
      ax = 0.5f * log((1 - fErr2) / fErr2);
      WeightsUpdate(fNewDataWeigths, fOldDataWeigths, fErr2, ErrH2, iErrNum2,
                    DATANUM);
    }

    if (iter == 1) {
      a1 = ax;
      ResultFinalL1 = (iMinErrNum == 1) ? H1(st, 6) : H2(dst, 1);
    } else if (iter == 0) {
      a2 = ax;
      ResultFinalL2 = (iMinErrNum == 1) ? H1(st, 6) : H2(dst, 1);
    }

    for (int i = 0; i < DATANUM; i++) {
      fOldDataWeigths[i] = fNewDataWeigths[i];
    }
  }

  return sign(a1 * ResultFinalL1 + a2 * ResultFinalL2);
}

// Helper function implementations
static int sign(float fx) { return (fx >= 0) ? 1 : -1; }

static int H1(float x, float TH) { return (x > TH) ? -1 : 1; }

static int H2(float x, float TH) { return (x > TH) ? -1 : 1; }

static int CompClassierDiff(int iInitialDataVal, int iClassierVal) {
  return (iInitialDataVal != iClassierVal) ? 1 : 0;
}

static int CountErrRightNum(const int Data[], int Num) {
  int CountNum = 0;
  for (int i = 0; i < Num; i++) {
    if (Data[i] != 0) {
      CountNum++;
    }
  }
  return CountNum;
}

static int CompMinErrValue(float err1, float err2) {
  return (err1 <= err2) ? 1 : 2;
}

static void WeightsUpdate(float fNewWcs[], const float fOldWcs[], float fErr,
                          const int ErrHx[], int ErrNumX, int DataNum) {
  for (int i = 0; i < DataNum; i++) {
    fNewWcs[i] = fOldWcs[i] / (2 * (1 - fErr));
  }

  for (int i = 0; i < ErrNumX; i++) {
    int idx = ErrHx[i] - 1;
    fNewWcs[idx] = fOldWcs[idx] / (2 * fErr);
  }
}

static float CalcNewClassifersErr(const float fOldDataWeigths[],
                                  const int ErrHx[], int ErrNumX) {
  float err = 0;
  for (int i = 0; i < ErrNumX; i++) {
    err += fOldDataWeigths[ErrHx[i] - 1];
  }
  return err;
}
