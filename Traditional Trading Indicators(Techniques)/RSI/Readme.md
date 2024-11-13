## RSI Based Trading Strategy

This project implements a trading strategy based on the RSI (Relative Strength Index) technical indicator. The RSI is a momentum oscillator that measures the speed and change of price movements. It ranges from 0 to 100 and is typically used to identify overbought or oversold conditions in a traded security.

### Overview

The RSI provides signals about bullish and bearish price momentum, and it is often plotted beneath the graph of an asset's price. 

In this strategy:
- A **buy** signal is generated when the RSI is between 40 and 45.
- A **sell** signal is generated when the RSI is between 45 and 50.

Furthermore, positions are closed based on additional RSI conditions:
- Buy positions are closed when the RSI exceeds 70 or falls below 30.
- Sell positions are closed when the RSI goes below 30 or exceeds 65.

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

The strategy's performance is visualized on a graph showing the stock's closing prices, the RSI, and buy/sell signals. The strategy's profitability is also calculated, providing insight into the number of profitable trades vs. stopped-out trades.

### Data

The strategy also saves the resulting dataframe with buy, sell, stop loss, and profit signals to a CSV file named 'azaz.csv'.
