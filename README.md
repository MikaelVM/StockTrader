# Stock Trading Library
Assignment for Selected Topics in Programming

Marius Mikučionis <marius@cs.aau.dk>

May 5, 2022
## Requirements 
The goal of the assignment is to develop a library for reading historical stock prices, simulating
algorithmic trades and evaluating profitability of various strategies. The library implementation
should support the following features:
1. Read the stock and trade information from JSON files.
2. The JSON parser should be generic (handle various data structures) and efficient (without
   copying an entire JSON file).
3. Compute candle-stick trajectories for a given selected period (e.g. one stick -- one day).
4. Compute stochastic indicators for given X and Y parameters from the candle-stick data.
5. Simulate a trade execution based on a given strategy based on a stochastic oscillator:
   a) An algorithm is given a fixed amount of money (e.g. 10000kr).
   b) Buy the stocks at current price after the oscillator signals “buy” and money is available.
   c) Sell the stocks at current price after the oscillator signals “sell” and get back the money.
   d) Repeat the last two steps until the trade data is over.
   e) At the end, sell all stocks (if any).
   f) Output the dates of trades and final profit/loss.
6. Use lambda expressions to either sort the trades, or create a stochastic indicator, or trade
   strategy.
7. Support and demonstrate parallel computation for either:
   a) Loading multiple stock data in parallel.
   b) Evaluating multiple trading strategies in parallel.
8. Use unit testing to demonstrate and test the parts of the library.
9. Benchmark: measure the time either for loading the stocks or simulating trade. Improve/vary
   the measured code/structures and measure again. Compare and make conclusion.
10. Visualize the stock price and oscillator signals by plotting them on screen.

## Further Info
https://www.moodle.aau.dk/pluginfile.php/2614717/mod_assign/intro/description.pdf