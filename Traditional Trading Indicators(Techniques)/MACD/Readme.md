
## MACD Based Trading Strategy

This project implements a trading strategy based on the MACD (Moving Average Convergence Divergence) technical indicator. MACD is used to identify changes in the strength, direction, momentum, and duration of a trend in a stock's price.

### Overview

The MACD consists of three components:

1. **MACD Line**: The difference between the 12-day and 26-day exponential moving averages.
2. **Signal Line**: A 9-day exponential moving average of the MACD line.
3. **MACD Histogram**: Represents the difference between the MACD line and the Signal line.

In this strategy:
- A **buy** signal is generated when the MACD line crosses above the Signal line.
- A **sell** signal is generated when the MACD line crosses below the Signal line.

The strategy also includes mechanisms to take profit or stop a loss based on the movement of the stock price after a position is taken.

### Libraries Used

- `pandas`: For data manipulation.
- `numpy`: For numerical operations.
- `matplotlib`: For visualization.
- `talib`: For technical analysis indicators.
- `yfinance`: For fetching historical stock data from Yahoo Finance.
- `datetime`: For date-time operations.

### How to Run

1. Ensure all required libraries are installed.
2. Set the desired stock symbol in the `symb` variable.
3. Execute the Python script to visualize the strategy on historical data.

### Results

The strategy's performance is visualized on a graph showing the stock's closing prices, the MACD, and buy/sell signals. The strategy's profitability is also calculated, providing insight into the number of profitable trades vs. stopped-out trades.
