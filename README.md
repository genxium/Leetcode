# Maximize/minimize "f(inputs)" by K times of using "operation(...)" on "inputs"

For the less difficult ones of these problems, they may have an "optimal substructure", e.g. "f(inputs)[k] = max{ operation(f(inputs)[j]) | j <= k-1 }", to exploit and we can just iterate K times to get the answer.
- [p787_Cheapest_Flights_Within_K_Stops](https://github.com/genxium/Leetcode/tree/master/p787_Cheapest_Flights_Within_K_Stops)
- [p1531_String_Compression_II](https://github.com/genxium/Leetcode/tree/master/p1531_String_Compression_II)

For the more difficult ones of these problems, they often don't have an "optimal substructure", or exploiting the "optimal substructure" isn't fast enough, hence demanding us to derive a "greedy step for current state". 
- [p188_Best_Time_to_Buy_and_Sell_Stock_IV](https://github.com/genxium/Leetcode/tree/master/p188_Best_Time_to_Buy_and_Sell_Stock_IV)
- [p1505_Minimum_Possible_Integer_After_at_Most_K_Adjacent_Swaps_On_Digits](https://github.com/genxium/Leetcode/tree/master/p1505_Minimum_Possible_Integer_After_at_Most_K_Adjacent_Swaps_On_Digits)

For some peculiar ones where K is very small but neither an "optimal substructure" nor an obvious "greedy step for current state" is available, we can just search exhaustively.
- [p1509_Minimum_Difference_Between_Largest_and_Smallest_Value_in_Three_Moves](https://github.com/genxium/Leetcode/tree/master/p1509_Minimum_Difference_Between_Largest_and_Smallest_Value_in_Three_Moves)
