## Stochastic Oscillator Based Trading Strategy

This project implements a trading strategy centered on the Stochastic Oscillator, a momentum indicator that compares a specific closing price of a security to its price range over a certain period of time.

### Overview

The Stochastic Oscillator can help determine overbought and oversold conditions. An asset is considered overbought when the oscillator is above 80 and oversold when it's below 20.

In this strategy:
- A **buy** signal is generated when the \( K \) line (the current value of the oscillator) crosses the \( D \) line (a moving average of the \( K \) line) and both are between 25 and 40.
- A **sell** signal is generated when the \( K \) line crosses below the \( D \) line and both are between 60 and 90.

Furthermore, open positions are closed based on additional Stochastic Oscillator conditions:
- Buy positions are closed when the \( D \) line exceeds 70 or falls below 20.
- Sell positions are closed when the \( D \) line drops below 30 or surpasses 90.

### Libraries Used

- `pandas`: For data manipulation and analysis.
- `numpy`: For numerical operations.
- `matplotlib`: For visualizations.
- `talib`: Technical analysis library used for calculating the Stochastic Oscillator.
- `yfinance`: For downloading historical stock data from Yahoo Finance.
- `datetime`: For date-time operations.

### How to Run

1. Ensure all the required libraries are installed.
2. Set the desired stock symbol in the `symb` variable.
3. Execute the Python script to visualize the strategy results on historical data.

### Results

The strategy's performance is depicted on a graph showing the stock's closing prices, the \( K \) and \( D \) lines, and buy/sell signals. Additionally, the strategy's profitability ratio is displayed, offering an insight into the number of profitable trades vs. stopped-out trades.

### Data Output

The strategy saves the resulting dataframe with buy, sell, stop loss, and profit signals in its entirety, allowing for further analysis or backtesting.
