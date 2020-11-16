Problems with similar solutions.
- [p312_Burst_Balloons](https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons)
- [p877_Stone_Game](https://github.com/genxium/Leetcode/tree/master/p877_Stone_Game)
- [p1547_Minimum_Cost_to_Cut_a_Stick](https://github.com/genxium/Leetcode/tree/master/p1547_Minimum_Cost_to_Cut_a_Stick)
- [p1563_Stone_Game_V](https://github.com/genxium/Leetcode/tree/master/p1563_Stone_Game_V)

Though being similar, this problem is a little more difficult than those mentioned above. If we just denote that `dp[l][r] == x` being the highest points achievable by bursting `boxes[l, ..., r]`, it'd be impossible to calculate `dp[l][r]` from `dp[l][k] and dp[k+1][r]` due to loss of color information, i.e. couldn't include the case where `boxes[l, ..., k]` and `boxes[k+1, ..., r]` are not independently fully bursted but only partially bursted with two segments of the same color remained. 

An intuitive idea is to denote that `dp[l][r][c] = {x, t}` being the "best" points "x" and remaining boxes of color "c" we can obtain by bursting "boxes[l, ..., r]" except for those of color "c", then calculate `dp[l][r][c]` from `(dp[l][k][c], dp[k+1][r][*])` and `dp[l][k][*], dp[k+1][r][c]`. Specifically, 
- in case there couldn't be any color "c" box remained by bursting "boxes[l, ..., r]", assign "x = 0 & t = 0",
- in case "c == 0" it means no box to remain by bursting "boxes[l, ..., r]", assign "t = 0" and maximize "x". Unfortunately this idea doesn't work when trying to compare `dp[l][r][c]` to a pair of `{candidateX, candidateT}`, it's difficult to justify a choice if `candidateX > dp[l][r][c].x && candidateT < dp[l][r][c].t`, checkout 
```
[5,8,8,6,9,8,6,9,5,8,6,5]
   ^               ^
   l               r

c == 5
``` 
as an example.
