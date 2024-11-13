#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define FILE_PATH "C:\\Users\\Daniel aghanya\\Pictures\\tiktok download\\Final_Year_Project-main\\Uni project\\Project\\aghanyan_Dissertation_Project_Files\\Data\\NSE All Share Historical Data Cleaned.csv"
#define MAX_SIZE 1653 // Assuming the maximum size of the data

// Function to compute the sigmoid function
double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

// Function to compute the logistic regression hypothesis
double hypothesis(double* theta, double* X, int n) {
    double z = theta[0]; // Bias term
    for (int i = 0; i < n; i++) {
        z += theta[i + 1] * X[i];
    }
    return sigmoid(z);
}

// Function to compute the cost
double cost(double* theta, double* X, double* y, int m, int n) {
    double cost = 0.0;
    for (int i = 0; i < m; i++) {
        double h = hypothesis(theta, &X[i * n], n);
        cost += -y[i] * log(h) - (1 - y[i]) * log(1 - h);
    }
    return cost / m;
}

// Function to compute the mean and standard deviation
void compute_mean_std(double* X, int m, int n, double* mean, double* std) {
    for (int j = 0; j < n; j++) {
        mean[j] = 0.0;
        std[j] = 0.0;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            mean[j] += X[i * n + j];
        }
    }

    for (int j = 0; j < n; j++) {
        mean[j] /= m;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std[j] += pow(X[i * n + j] - mean[j], 2);
        }
    }

    for (int j = 0; j < n; j++) {
        std[j] = sqrt(std[j] / (m - 1));
    }
}

// Function to standardize the data
void standardize_data(double* X, int m, int n, double* mean, double* std) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            X[i * n + j] = (X[i * n + j] - mean[j]) / std[j];
        }
    }
}

int main() {
    int m = 200; // Number of training examples
    int n = 1; // Number of features (close prices)

    double X[200] = {1269640, 1269710, 1270850, 1270710, 1270240, 1269550, 1269400, 1268910, 1268120, 1268680, 1268570, 1268050, 1267940, 1267630, 1267170, 1266930, 1268270, 1268550, 1268980, 1269730, 1269370, 1269990, 1269870, 1270440, 1270010, 1270020, 1270610, 1271850, 1272730, 1272520, 1272360, 1271850, 1272320, 1273170, 1274850, 1274520, 1273680, 1274070, 1272550, 1274980, 1273530, 1270560, 1269560, 1269000, 1269680, 1270390, 1270250, 1269320, 1268430, 1268910, 1268790, 1267800, 1267300, 1266750, 1266860, 1266550, 1267160, 1267470, 1266330, 1266590, 1266070, 1265740, 1265000, 1265390, 1264620, 1264320, 1263300, 1263530, 1263500, 1263680, 1263950, 1263710, 1264020, 1263570, 1262930, 1261810, 1261320, 1261090, 1262000, 1263060, 1262810, 1264250, 1263250, 1264300, 1263240, 1263120, 1264040, 1268190, 1269520, 1268010, 1269410, 1269460, 1268390, 1267760, 1268260, 1269140, 1269240, 1271200, 1271140, 1270610, 1269920, 1269600, 1269410, 1269110, 1269410, 1271260, 1269760, 1270370, 1269960, 1270460, 1269200};
    double y[200]; // Dummy target variable (0 or 1)

    // Initialize dummy target variable with random 0s and 1s
    srand(time(NULL));
    for (int i = 0; i < m; i++) {
        y[i] = rand() % 2;
    }

    double split_percentage = 0.8;
    int split = (int)(split_percentage * m);

    // Split data into train and test sets
    double X_train[split * n];
    double y_train[split];
    double X_test[m * n - split * n];
    double y_test[m - split];

    for (int i = 0; i < split; i++) {
        y_train[i] = y[i];
        for (int j = 0; j < n; j++) {
            X_train[i * n + j] = X[i * n + j];
        }
    }

    for (int i = split; i < m; i++) {
        y_test[i - split] = y[i];
        for (int j = 0; j < n; j++) {
            X_test[(i - split) * n + j] = X[i * n + j];
        }
    }

    // Standardize the data
    double mean_train[n], std_train[n];
    compute_mean_std(X_train, split, n, mean_train, std_train);
    standardize_data(X_train, split, n, mean_train, std_train);
    standardize_data(X_test, m - split, n, mean_train, std_train);

    double* theta = (double*)malloc((n + 1) * sizeof(double)); // Allocate memory for theta
    double* gradient = (double*)malloc((n + 1) * sizeof(double)); // Allocate memory for gradient
    double alpha = 0.01; // Learning rate

    // Initialize theta and gradient to 0
    for (int i = 0; i < n + 1; i++) {
        theta[i] = 0.0;
        gradient[i] = 0.0;
    }

    clock_t start = clock();

    // Train the logistic regression model
    int max_iter = 1000;
    for (int iter = 0; iter < max_iter; iter++) {
        double cost_val = cost(theta, X_train, y_train, split, n + 1);

        // Perform parameter updates using gradient descent
        for (int j = 0; j < split; j++) {
            double h = hypothesis(theta, &X_train[j * n], n);
            double error = h - y_train[j];
            gradient[0] += error;
            for (int i = 0; i < n; i++) {
                gradient[i + 1] += error * X_train[j * n + i];
            }
        }
        for (int i = 0; i < n + 1; i++) {
            theta[i] -= alpha * gradient[i] / split;
            gradient[i] = 0.0; // Reset gradient to 0
        }
    }

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Training time: %f seconds\n", time_taken);
    printf("Press Enter to exit...");
    getchar();
    free(theta); // Free the dynamically allocated memory
    free(gradient);

    return 0;
}