import csv
import time
import math
from concurrent.futures import ThreadPoolExecutor

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

# The rest of your calculations here, adjusting the length dynamically
length = len(Close_prices)
gain = [0] * length
loss = [0] * length
difference = [0] * length
Average_gain = [0] * length
Average_loss = [0] * length
RS = [0] * length
RSI = [0] * length
buy = [0] * length
sell = [0] * length

def calculate_rsi_chunk(close_prices, period=10):
    gains = []
    losses = []
    # Calculate gains and losses
    for i in range(1, len(close_prices)):
        change = close_prices[i] - close_prices[i - 1]
        if change > 0:
            gains.append(change)
            losses.append(0)
        else:
            gains.append(0)
            losses.append(abs(change))

    # Calculate average gains and losses
    avg_gains = []
    avg_losses = []
    for i in range(period, len(gains) + 1):
        avg_gain = sum(gains[i-period:i]) / period
        avg_loss = sum(losses[i-period:i]) / period
        avg_gains.append(avg_gain)
        avg_losses.append(avg_loss)

    # Calculate RS and RSI
    rsi = []  # Corrected variable name here
    for i in range(len(avg_gains)):
        RS = avg_gains[i] / avg_losses[i] if avg_losses[i] != 0 else 0
        rsi_value = 100 - (100 / (1 + RS)) if avg_losses[i] != 0 else 100
        rsi.append(rsi_value)

    # Prepend with NaNs to make the length match with the input close_prices
    rsi = [None] * (period - 1) + rsi
    return rsi 


# Function to divide Close_prices into chunks
def divide_chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

# Divide Close_prices into chunks for multithreading
close_prices_chunks = list(divide_chunks(Close_prices, len(Close_prices) // 4))

# Use ThreadPoolExecutor to calculate RSI for each chunk
rsi_chunks = []
with ThreadPoolExecutor(max_workers=4) as executor:
    futures = [executor.submit(calculate_rsi_chunk, chunk) for chunk in close_prices_chunks]
    for future in futures:
        rsi_chunks.append(future.result())
        
# Combine RSI chunks back into a single list
rsi = [item for sublist in rsi_chunks for item in sublist]
position_Buy = None
position_Sell = None
buy_signals = []
sell_signals = []
Stop_loss = []
Profit = []

for i in range(len(rsi)):
    if rsi[i] is None:  # Skip the loop iteration if rsi[i] is None
        buy_signals.append(None)
        sell_signals.append(None)
        Stop_loss.append(None)
        Profit.append(None)
        continue  # Continue to the next iteration
    ###################################### SIGNAL FOR BUY #######################################################
    if rsi[i] < 45 and rsi[i] >= 40 and position_Buy != "Buy" and position_Sell != "Sell":
        buy.append(rsi[i])
        sell.append(math.nan)
        Stop_loss.append(math.nan)
        Profit.append(math.nan)
        position_Buy = "Buy"

    ###################################### SIGNAL FOR SELL #######################################################
    elif rsi[i] <= 50 and rsi[i] > 45 and position_Sell != "Sell" and position_Buy != "Buy":
        sell.append(rsi[i])
        buy.append(math.nan)
        Stop_loss.append(math.nan)
        Profit.append(math.nan)
        position_Sell = "Sell"

    ###################################### SIGNAL FOR BUY_rsi #######################################################
    elif position_Buy == "Buy":
        if rsi[i] >= 70:
            Stop_loss.append(math.nan)
            Profit.append(rsi[i])
            buy.append(math.nan)
            sell.append(math.nan)
            position_Buy = None

        elif rsi[i] <= 30:
            Stop_loss.append(rsi[i])
            Profit.append(math.nan)
            buy.append(math.nan)
            sell.append(math.nan)
            position_Buy = None

        else:
            buy.append(math.nan)
            sell.append(math.nan)
            Stop_loss.append(math.nan)
            Profit.append(math.nan)

    ###################################### SIGNAL FOR SELL_rsi #######################################################
    elif position_Sell == "Sell":
        if rsi[i] <= 30:
            Stop_loss.append(math.nan)
            Profit.append(rsi[i])
            buy.append(math.nan)
            sell.append(math.nan)
            position_Sell = None

        elif rsi[i] >= 65:
            Stop_loss.append(rsi[i])
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
        
# Printing counts and profitability as per your request
profit_count = sum(1 for x in Profit if x is not None and not math.isnan(x))
stop_loss_count = sum(1 for x in Stop_loss if x is not None and not math.isnan(x))

total_events = profit_count + stop_loss_count
profitability = (profit_count / total_events) * 100 if total_events > 0 else 0

print('Profit =', profit_count)
print('Stop_loss =', stop_loss_count)
print(f"PROFITABILITY {profitability}%")

end_time = time.time()
elapsed_time = end_time - start_time
print(f"Time taken: {elapsed_time} seconds")
