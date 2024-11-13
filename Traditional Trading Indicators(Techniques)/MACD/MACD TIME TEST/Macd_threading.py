import time
from threading import Thread

def calculate_ema12(Close_prices, K1, EMA12):
    for i in range(len(Close_prices)):
        if i < 12:
            sum_ema12 = sum(Close_prices[:12])
            EMA12.append(sum_ema12 / 12)
        else:
            ema12 = (Close_prices[i] * K1) + (EMA12[-1] * (1 - K1))
            EMA12.append(ema12)

def calculate_ema26(Close_prices, K2, EMA26):
    for i in range(len(Close_prices)):
        if i < 26:
            if i == 25:  # Ensure we calculate the initial EMA26 only once
                sum_ema26 = sum(Close_prices[:26])
                EMA26.append(sum_ema26 / 26)
        else:
            ema26 = (Close_prices[i] * K2) + (EMA26[-1] * (1 - K2))
            EMA26.append(ema26)

def calculate_macd(EMA12, EMA26, MACD):
    for i in range(len(EMA26)):  # Adjusted to loop through EMA26 length
        macd = EMA12[i + 14] - EMA26[i]  # Adjusted indexing to match lengths
        MACD.append(macd)

def calculate_macd_signal(MACD, K3, MACD_SIGNAL):
    for i in range(len(MACD)):
        if i < 9:
            sum_macd_signal = sum(MACD[:9])
            MACD_SIGNAL.append(sum_macd_signal / 9)
        else:
            macd_signal = (MACD[i] * K3) + (MACD_SIGNAL[-1] * (1 - K3))
            MACD_SIGNAL.append(macd_signal)

def identify_signals(Close_prices, MACD, MACD_SIGNAL, buy, sell):
    position_buy = True
    position_sell = False
    for i in range(len(MACD)):
        if MACD[i] > MACD_SIGNAL[i] and position_buy:
            buy.append((i, MACD[i]))  # Example of using index and value
            position_sell = True
            position_buy = False
        elif MACD[i] < MACD_SIGNAL[i] and position_sell:
            sell.append((i, MACD[i]))  # Example of using index and value
            position_sell = False
            position_buy = True

start_time = time.time()

Close_prices = [1269640, 1269710, 1270850, 1270710, 1270240, 1269550, 1269400, 1268910, 1268120, 1268680, 1268570, 1268050, 1267940, 1267630, 1267170, 1266930, 1268270, 1268550, 1268980, 1269730, 1269370, 1269990, 1269870, 1270440, 1270010, 1270020, 1270610, 1271850, 1272730, 1272520, 1272360, 1271850, 1272320, 1273170, 1274850, 1274520, 1273680, 1274070, 1272550, 1274980, 1273530, 1270560, 1269560, 1269000, 1269680, 1270390, 1270250, 1269320, 1268430, 1268910, 1268790, 1267800, 1267300, 1266750, 1266860, 1266550, 1267160, 1267470, 1266330, 1266590, 1266070, 1265740, 1265000, 1265390, 1264620, 1264320, 1263300, 1263530, 1263500, 1263680, 1263950, 1263710, 1264020, 1263570, 1262930, 1261810, 1261320, 1261090, 1262000, 1263060, 1262810, 1264250, 1263250, 1264300, 1263240, 1263120, 1264040, 1268190, 1269520, 1268010, 1269410, 1269460, 1268390, 1267760, 1268260, 1269140, 1269240, 1271200, 1271140, 1270610, 1269920, 1269600, 1269410, 1269110, 1269410, 1271260, 1269760, 1270370, 1269960, 1270460, 1269230, 1271840, 1273020, 1272610, 1271380, 1270730, 1270670, 1270050, 1269590, 1269950, 1270310, 1270020, 1269110, 1270140, 1269270, 1270700, 1270580, 1270060, 1270470, 1270500, 1268540, 1268460, 1269370, 1268630, 1267890, 1268340, 1270150, 1272070, 1270630, 1269920, 1270280, 1267840, 1267760, 1267810, 1267880, 1268130, 1267320, 1269840, 1271040, 1271150, 1270220, 1269530, 1269250, 1268790, 1266100, 1265420, 1263790, 1263610, 1261130, 1260710, 1261160, 1260870, 1259550, 1260800, 1261830, 1262140, 1262150, 1261490, 1261200, 1260080, 1259520, 1259210, 1260750, 1260770, 1260230, 1260240, 1260630, 1263800, 1264200, 1263550, 1263050, 1262850, 1262600, 1262480, 1262840, 1261580, 1260650, 1259570, 1260150, 1260210, 1261950, 1261750, 1261060, 1261140, 1260750, 1258680, 1259140, 1259670, 1258620, 1258820]

EMA12 = []
EMA26 = []
MACD = []
MACD_SIGNAL = []
buy = []
sell = []

K1 = 0.1538461538
K2 = 0.0740740741
K3 = 0.2

# Start threads for EMA12 and EMA26 calculations
thread1 = Thread(target=calculate_ema12, args=(Close_prices, K1, EMA12))
thread2 = Thread(target=calculate_ema26, args=(Close_prices, K2, EMA26))
thread1.start()
thread2.start()
thread1.join()
thread2.join()

# After EMA12 and EMA26 are calculated, start MACD and MACD_SIGNAL calculations
thread3 = Thread(target=calculate_macd, args=(EMA12, EMA26, MACD))
thread4 = Thread(target=calculate_macd_signal, args=(MACD, K3, MACD_SIGNAL))
thread3.start()
thread4.start()
thread3.join()
thread4.join()

# Now that we have MACD and MACD_SIGNAL, identify buy and sell signals
buy = []
sell = []
identify_signals(Close_prices, MACD, MACD_SIGNAL, buy, sell)

end_time = time.time()
elapsed_time = end_time - start_time
print(f"Time taken (threaded): {elapsed_time} seconds")
