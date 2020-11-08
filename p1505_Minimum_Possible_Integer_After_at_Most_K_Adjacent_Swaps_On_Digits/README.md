Let's first show why a greedy approach gives the correct answer. For 2 arrays of the same size, it's obvious that the one with smaller digits on the left is "lexicographically smaller", e.g.

`A = [1, 2, 42, 95, 96, 97, 98, 99, 94, 3, 11]` is smaller than
`B = [1, 2, 94, 3, 95, 96, 97, 98, 99, 42, 11]`

Therefore when the state is `O = [1, 2, 94, 95, 96, 97, 98, 99, 42, 3, 11], k = 6`, the largest we can obtain by swapping adjacent nums will be `A`.

Next, we need a method to "findSmallest(vector<int> &arr, int startOffset, int k)". The "SegmentTree" solution here might look a bit unfamiliar to some, please read [this note](https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e) and checkout https://github.com/genxium/Leetcode/tree/master/p307_Range_Sum_Query_Mutable beforehand to get more comfortable about it.
