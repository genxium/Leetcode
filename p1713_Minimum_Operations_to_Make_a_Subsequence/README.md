The key idea is that "LongestCommonSubsequence(LCS)" of 2 strings "foo" and "bar" where "foo" has only distinct elements, can be found in O(nlogn) time-complexity by
- create a map "u[]" where for each "bar[i]" it suffices that "u[i] == foo.indexOf(bar[i])" 
- the length of the "LongestIncreasingSubsequence(LIS)" of "u[]" will be the length of the LCS of "foo" and "bar".

Related problems
- [p1143_Longest_Common_Subsequence](https://github.com/genxium/Leetcode/tree/master/p1143_Longest_Common_Subsequence) 
- [p300_Longest_Increasing_Subsequence](https://github.com/genxium/Leetcode/tree/master/p300_Longest_Increasing_Subsequence)

