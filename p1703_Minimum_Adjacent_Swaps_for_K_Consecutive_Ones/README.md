This problem looks like 
- [p1547_Minimum_Cost_to_Cut_a_Stick](https://github.com/genxium/Leetcode/tree/master/p1547_Minimum_Cost_to_Cut_a_Stick)
- [p312_Burst_Balloons](https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons)
for the first sight, yet quite different in the characteristics of "MergeCost" (not just different in the max size of input). 

For string "x...x1...10...01...1x...x" where "x" might be either "1" or "0", we can transform it to either
- x...x1......10...0x...x, or
- x...x0...01......1x...x, or 
- anything in between that interleaves the "0"s and "1"s

, and each transformation makes new "MergeCost"s for all the subsequent operations.

This problem is actually similar to 
- [p462_Minimum_Moves_to_Equal_Array_Elements_II](https://github.com/genxium/Leetcode/tree/master/p462_Minimum_Moves_to_Equal_Array_Elements_II)
where the "median position" plays a vital role and needs a mathematical proof for correctness. 
