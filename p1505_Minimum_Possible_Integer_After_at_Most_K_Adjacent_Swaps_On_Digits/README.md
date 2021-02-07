Let's first show why a "greedy approach(i.e. seeking the min in range [0, remainingSteps] of remaining elements and swap it to the front, then offset by 1)" gives the correct answer. For 2 arrays of the same size, it's obvious that the one with smaller digits on the left is "lexicographically smaller", e.g.

`A = [1, 2, 42, 95, 96, 97, 98, 99, 94, 3, 11]` is smaller than
`B = [1, 2, 94, 3, 95, 96, 97, 98, 99, 42, 11]`

Therefore when the state is `O = [1, 2, 94, 95, 96, 97, 98, 99, 42, 3, 11], k = 6`, the smallest we can obtain by swapping adjacent nums will be `A`.

Next, we need a method to "findSmallest(vector<int> &arr, int startOffset, int k)". The "SegmentTree" solution here might look a bit unfamiliar to some, please read [this note](https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e) and checkout https://github.com/genxium/Leetcode/tree/master/p307_Range_Sum_Query_Mutable beforehand to get more comfortable about it.

Similar problems
- [p699_Falling_Squares](https://github.com/genxium/Leetcode/tree/master/p699_Falling_Squares), similar in terms of the use of SegmentTree.  
- [p188_Best_Time_to_Buy_and_Sell_Stock_IV](https://github.com/genxium/Leetcode/tree/master/p188_Best_Time_to_Buy_and_Sell_Stock_IV)
- [p787_Cheapest_Flights_Within_K_Stops](https://github.com/genxium/Leetcode/tree/master/p787_Cheapest_Flights_Within_K_Stops)
- [p1531_String_Compression_II](https://github.com/genxium/Leetcode/tree/master/p1531_String_Compression_II)
- [p1751_Maximum_Number_of_Events_That_Can_Be_Attended_II](https://github.com/genxium/Leetcode/tree/master/p1751_Maximum_Number_of_Events_That_Can_Be_Attended_II)
