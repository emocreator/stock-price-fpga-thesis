import csv
import time
import math
from concurrent.futures import ThreadPoolExecutor

def ema(values, period):
    """Calculates the Exponential Moving Average of a list."""
    ema_values = [values[0]]  # Start with the first value for initial EMA
    multiplier = 2 / (period + 1)
    for i in range(1, len(values)):
        ema_next = (values[i] - ema_values[-1]) * multiplier + ema_values[-1]
        ema_values.append(ema_next)
    return ema_values

def calculate_macd(close_prices, fastperiod=12, slowperiod=26, signalperiod=9):
    """Calculate MACD and MACD Signal."""
    ema_fast = ema(close_prices, fastperiod)
    ema_slow = ema(close_prices, slowperiod)
    macd = [fast - slow for fast, slow in zip(ema_fast[len(ema_fast)-len(ema_slow):], ema_slow)]
    macd_signal = ema(macd, signalperiod)
    macd_hist = [m - s for m, s in zip(macd[len(macd)-len(macd_signal):], macd_signal)]
    return macd[len(macd)-len(macd_signal):], macd_signal, macd_hist

start_time = time.time()

# Path to your CSV file
file_path = 'C:\\Users\\Daniel aghanya\\Pictures\\tiktok download\\Final_Year_Project-main\\Uni project\\Project\\aghanyan_Dissertation_Project_Files\\Data\\NSE All Share Historical Data Cleaned.csv'

# Initialize the Close_prices list
Close_prices = []

# Read Close prices from the CSV file
with open(file_path, mode='r') as csvfile:
    csvreader = csv.DictReader(csvfile)
    for row in csvreader:
        Close_prices.append(float(row['close']))  # Assuming the column name is 'close'

print(Close_prices)
macd, macd_signal, macd_hist = calculate_macd(Close_prices)

# Initialize signals and positions
buy, sell, Profit, Stop_loss, Close_order_buy, Close_order_sell = [], [], [], [], [], []
position_Buy, position_Sell = None, None

# Initialize the lists for trading signals and positions
buy = []
sell = []
Profit = []
Stop_loss = []
Close_order_buy = []
Close_order_sell = []
position_Buy = None
position_Sell = None

for i in range(len(macd)):
    if macd[i] > macd_signal[i] and position_Buy != "Buy" and position_Sell != "Sell":
        buy.append(macd[i])
        sell.append(math.nan)  # Using math.nan to represent missing values
        Stop_loss.append(math.nan)
        Profit.append(math.nan)
        Close_order_buy.append(macd[i])
        position_Buy = "Buy"
    elif macd[i] < macd_signal[i] and position_Sell != "Sell" and position_Buy != "Buy":
        sell.append(macd[i])
        buy.append(math.nan)
        Stop_loss.append(math.nan)
        Close_order_sell.append(macd[i])
        Profit.append(math.nan)
        position_Sell = "Sell"
    elif position_Buy == "Buy" and macd[i] < macd_signal[i]:
        if macd[i] > Close_order_buy[-1]:
            Stop_loss.append(math.nan)
            Profit.append(macd[i])
            buy.append(math.nan)
            sell.append(math.nan)
            position_Buy = None
        elif macd[i] < Close_order_buy[-1]:
            Stop_loss.append(macd[i])
            Profit.append(math.nan)
            buy.append(math.nan)
            sell.append(math.nan)
            position_Buy = None
        else:
            buy.append(math.nan)
            sell.append(math.nan)
            Stop_loss.append(math.nan)
            Profit.append(math.nan)
    elif position_Sell == "Sell" and macd[i] > macd_signal[i]:
        if macd[i] < Close_order_sell[-1]:
            Stop_loss.append(math.nan)
            Profit.append(macd[i])
            buy.append(math.nan)
            sell.append(math.nan)
            position_Sell = None
        elif macd[i] > Close_order_sell[-1]:
            Stop_loss.append(macd[i])
            Profit.append(math.nan)
            buy.append(math.nan)
            sell.append(math.nan)
            position_Sell = None
        else:
            buy.append(math.nan)
            sell.append(math.nan)
            Stop_loss.append(math.nan)
            Profit.append(math.nan)
    else:
        buy.append(math.nan)
        sell.append(math.nan)
        Stop_loss.append(math.nan)
        Profit.append(math.nan)

# Calculate profitability
profit_count = sum(1 for x in Profit if not math.isnan(x))
stop_loss_count = sum(1 for x in Stop_loss if not math.isnan(x))
total_events = profit_count + stop_loss_count
profitability = (profit_count / total_events) * 100 if total_events > 0 else 0

print('Profit =', profit_count)
print('Stop_loss =', stop_loss_count)
print(f"PROFITABILITY {profitability}%")

end_time = time.time()
elapsed_time = end_time - start_time
print(f"Time taken: {elapsed_time} seconds")
