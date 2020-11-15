Problems with similar solutions.
- [p312_Burst_Balloons](https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons)
- [p1547_Minimum_Cost_to_Cut_a_Stick](https://github.com/genxium/Leetcode/tree/master/p1547_Minimum_Cost_to_Cut_a_Stick)

Though being similar, this problem is a little more difficult than [p312_Burst_Balloons](https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons) and [p1547_Minimum_Cost_to_Cut_a_Stick](https://github.com/genxium/Leetcode/tree/master/p1547_Minimum_Cost_to_Cut_a_Stick). If we just denote that `dp[l][r] == x` being the highest points achievable by bursting `boxes[l, ..., r]`, it'd be impossible to calculate `dp[l][r]` from `dp[l][k] and dp[k+1][r]` due to loss of color information, i.e. couldn't include the case where `boxes[l, ..., k]` and `boxes[k+1, ..., r]` are not independently fully bursted but only partially bursted with two segments of the same color remained. 
