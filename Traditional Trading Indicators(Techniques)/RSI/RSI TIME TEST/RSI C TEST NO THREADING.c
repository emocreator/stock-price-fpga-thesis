#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define FILE_PATH "C:\\Users\\Daniel aghanya\\Pictures\\tiktok download\\Final_Year_Project-main\\Uni project\\Project\\aghanyan_Dissertation_Project_Files\\Data\\NSE All Share Historical Data Cleaned.csv"
#define MAX_SIZE 1653 // Assuming the maximum size of the data
#define PERIOD 10

float Close_prices[MAX_SIZE];
float rsi[MAX_SIZE];
float buy[MAX_SIZE];
float sell[MAX_SIZE];
float Stop_loss[MAX_SIZE];
float Profit[MAX_SIZE];
int length = 0;

void initialize_arrays() {
    for (int i = 0; i < MAX_SIZE; i++) {
        buy[i] = -1;
        sell[i] = -1;
        Stop_loss[i] = -1;
        Profit[i] = -1;
    }
}

void process_signals() {
    int position_Buy = 0; // 0 means no position, 1 means position
    int position_Sell = 0; // 0 means no position, 1 means position

    for (int i = PERIOD; i < length; i++) { // Start from PERIOD because RSI calculation starts filling from there
        if (rsi[i] < 45 && rsi[i] >= 40 && position_Buy == 0 && position_Sell == 0) {
            buy[i] = rsi[i];
            position_Buy = 1;
        } else if (rsi[i] <= 50 && rsi[i] > 45 && position_Sell == 0 && position_Buy == 0) {
            sell[i] = rsi[i];
            position_Sell = 1;
        } else if (position_Buy == 1) {
            if (rsi[i] >= 70) {
                Profit[i] = rsi[i];
                position_Buy = 0;
            } else if (rsi[i] <= 30) {
                Stop_loss[i] = rsi[i];
                position_Buy = 0;
            }
        } else if (position_Sell == 1) {
            if (rsi[i] <= 30) {
                Profit[i] = rsi[i];
                position_Sell = 0;
            } else if (rsi[i] >= 65) {
                Stop_loss[i] = rsi[i];
                position_Sell = 0;
            }
        }
    }
}

int count_signals(float array[]) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (array[i] != -1) {
            count++;
        }
    }
    return count;
}

float calculate_profitability() {
    int profit_count = count_signals(Profit);
    int stop_loss_count = count_signals(Stop_loss);
    int total_events = profit_count + stop_loss_count;
    if (total_events > 0) {
        return (float)profit_count / total_events * 100;
    }
    return 0;
}

void calculate_rsi(float close_prices[], int period) {
    float gains[MAX_SIZE] = {0};
    float losses[MAX_SIZE] = {0};
    float avg_gains[MAX_SIZE] = {0};
    float avg_losses[MAX_SIZE] = {0};
    float RS, rsi_value;

    for (int i = 1; i < length; i++) {
        float change = close_prices[i] - close_prices[i - 1];
        if (change > 0) {
            gains[i] = change;
            losses[i] = 0;
        } else {
            gains[i] = 0;
            losses[i] = fabs(change);
        }
    }

    for (int i = period; i <= length; i++) {
        float sum_gains = 0, sum_losses = 0;
        for (int j = i - period; j < i; j++) {
            sum_gains += gains[j];
            sum_losses += losses[j];
        }
        avg_gains[i - 1] = sum_gains / period;
        avg_losses[i - 1] = sum_losses / period;
    }

    for (int i = period - 1; i < length; i++) {
        if (avg_losses[i] != 0) {
            RS = avg_gains[i] / avg_losses[i];
            rsi_value = 100 - (100 / (1 + RS));
        } else {
            rsi_value = 100;
        }
        rsi[i] = rsi_value;
    }
}

int main() {
    clock_t start_time, end_time;
    start_time = clock();

    FILE *csvfile;
    char buffer[1024];
    int row_count = 0;
    int field_count = 0;

    csvfile = fopen(FILE_PATH, "r");
    if (!csvfile) {
        printf("Could not open file %s", FILE_PATH);
        return 1;
    }

    while (fgets(buffer, 1024, csvfile)) {
        field_count = 0;
        row_count++;
        if (row_count == 1) continue; // Skip header

        char *field = strtok(buffer, ",");
        while (field) {
            if (field_count == 7) { // Assuming 'close' is the 5th column
                Close_prices[length++] = atof(field);
            }
            field = strtok(NULL, ",");
            field_count++;
        }
    }
    fclose(csvfile);

    calculate_rsi(Close_prices, 10);

    initialize_arrays(); // Initialize buy, sell, Stop_loss, and Profit arrays
    process_signals(); // Process buy and sell signals based on RSI values
    float profitability = calculate_profitability();

    printf("Profit: %d\n", count_signals(Profit));
    printf("Stop_loss: %d\n", count_signals(Stop_loss));
    printf("Profitability: %.2f%%\n", profitability);

    end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);

    return 0;
}


