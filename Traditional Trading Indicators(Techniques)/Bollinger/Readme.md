## Bollinger Bands Based Trading Strategy

This project implements a trading strategy based on the Bollinger Bands technical indicator. Bollinger Bands are used to identify overbought and oversold conditions in the market, helping traders make informed decisions.

### Overview

The Bollinger Bands consist of three lines:

1. **Middle Band**: A simple moving average.
2. **Upper Band**: Represents a standard deviation above the middle band.
3. **Lower Band**: Represents a standard deviation below the middle band.

In this strategy:
- A **buy** signal is generated when the stock price falls below the lower band.
- A **sell** signal is generated when the stock price goes above the upper band.

Additionally, the strategy implements a take-profit and stop-loss mechanism to optimize returns and minimize losses.

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

The strategy's performance is visualized on a graph showing the stock's closing prices, the Bollinger Bands, and buy/sell signals. The strategy's profitability is also calculated, providing insight into the number of profitable trades vs. stopped-out trades.
