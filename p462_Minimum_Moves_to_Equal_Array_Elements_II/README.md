A simple proof for why median is the best to minimize "sum(|a[i]-x|)".

Note that "d|x|/dx = sign(x)", "d(sum(|a[i]-x|))/dx = sum(sign(a[i]-x))", which is only 0 when "x == median(a[])".  

Similar problems
- [p1703_Minimum_Adjacent_Swaps_for_K_Consecutive_Ones](https://github.com/genxium/Leetcode/tree/master/p1703_Minimum_Adjacent_Swaps_for_K_Consecutive_Ones)
