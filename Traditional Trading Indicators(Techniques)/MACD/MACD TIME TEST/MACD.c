#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define FILE_PATH "C:\\Users\\Daniel aghanya\\Pictures\\tiktok download\\Final_Year_Project-main\\Uni project\\Project\\aghanyan_Dissertation_Project_Files\\Data\\NSE All Share Historical Data Cleaned.csv"
#define MAX_SIZE 1653 // Assuming the maximum size of the data
#define FAST_PERIOD 12
#define SLOW_PERIOD 26
#define SIGNAL_PERIOD 9

float Close_prices[MAX_SIZE];
float MACD[MAX_SIZE];
float Signal[MAX_SIZE];
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

    for (int i = SLOW_PERIOD + SIGNAL_PERIOD; i < length; i++) {
        // Buy signal condition
        if (MACD[i] > Signal[i] && position_Buy == 0 && position_Sell == 0) {
            buy[i] = MACD[i];
            sell[i] = -1;
            Stop_loss[i] = -1;
            Profit[i] = -1;
            position_Buy = 1;
        }
        // Sell signal condition
        else if (MACD[i] < Signal[i] && position_Sell == 0 && position_Buy == 0) {
            sell[i] = MACD[i];
            buy[i] = -1;
            Stop_loss[i] = -1;
            Profit[i] = -1;
            position_Sell = 1;
        }
        // Closing Buy position condition
        else if (position_Buy == 1 && MACD[i] < Signal[i]) {
            if (MACD[i] > buy[i]) {
                Profit[i] = MACD[i];
                Stop_loss[i] = -1;
            } else {
                Stop_loss[i] = MACD[i];
                Profit[i] = -1;
            }
            buy[i] = -1;
            sell[i] = -1;
            position_Buy = 0;
        }
        // Closing Sell position condition
        else if (position_Sell == 1 && MACD[i] > Signal[i]) {
            if (MACD[i] < sell[i]) {
                Profit[i] = MACD[i];
                Stop_loss[i] = -1;
            } else {
                Stop_loss[i] = MACD[i];
                Profit[i] = -1;
            }
            buy[i] = -1;
            sell[i] = -1;
            position_Sell = 0;
        }
        else {
            buy[i] = -1;
            sell[i] = -1;
            Stop_loss[i] = -1;
            Profit[i] = -1;
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
// Function to calculate EMA
float EMA(float prices[], int size, int period, int startFrom) {
    float ema = prices[startFrom];
    float multiplier = 2.0 / (period + 1);
    for (int i = startFrom + 1; i < startFrom + period; i++) {
        ema = (prices[i] - ema) * multiplier + ema;
    }
    return ema;
}

// Function to calculate MACD values
void calculate_MACD() {
    // Initial EMA calculations for the starting point
    float sumFast = 0.0, sumSlow = 0.0;
    for (int i = 0; i < FAST_PERIOD; i++) sumFast += Close_prices[i];
    for (int i = 0; i < SLOW_PERIOD; i++) sumSlow += Close_prices[i];

    float fastEMA = sumFast / FAST_PERIOD;
    float slowEMA = sumSlow / SLOW_PERIOD;

    // MACD calculation
    for (int i = SLOW_PERIOD; i < length; i++) {
        fastEMA = (Close_prices[i] - fastEMA) * (2.0 / (FAST_PERIOD + 1)) + fastEMA;
        slowEMA = (Close_prices[i] - slowEMA) * (2.0 / (SLOW_PERIOD + 1)) + slowEMA;
        MACD[i] = fastEMA - slowEMA;
    }

    // Signal line calculation
    float sumSignal = 0.0;
    for (int i = SLOW_PERIOD; i < SLOW_PERIOD + SIGNAL_PERIOD; i++) sumSignal += MACD[i];
    Signal[SLOW_PERIOD + SIGNAL_PERIOD - 1] = sumSignal / SIGNAL_PERIOD;

    for (int i = SLOW_PERIOD + SIGNAL_PERIOD; i < length; i++) {
        Signal[i] = (MACD[i] - Signal[i-1]) * (2.0 / (SIGNAL_PERIOD + 1)) + Signal[i-1];
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

    initialize_arrays(); // Initialize buy, sell, Stop_loss, and Profit arrays
    calculate_MACD();

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


