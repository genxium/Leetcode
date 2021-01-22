At first sight this is a "topological sorting" problem, however the input doesn't immediately conform to a graph and to build such graph it'll take O(n^2) time.

Without loss of generality, I choose to maintain a "sortedCache" with "decreasing width". Moreover, let's say that if "A={w1, h1}" and "B={w2, h2}" suffice that "w1 > w2 && h1 < h2", then "A and B forms a tie".

Consider first that we use an approach exactly analogous to [LongestIncreasingSubsequence(LIS)](https://github.com/genxium/Leetcode/tree/master/p300_Longest_Increasing_Subsequence), i.e. maintaining the "sortedCache" WITHOUT sorting the input "envelopes". This is completely unacceptable, just consider that the original order of "envelopes" is like [{99, 6}, {102, 102}, {98, 5}, {101, 101}, {97, 4} {100, 7}] -- we'll only get about half of the longest possible answer.

Therefore let's first sort "envelopes" by decending "width", but under the same "width", "height" might be randomly ordered.
- At some moment of time we might have to evaluate "envelope={100, 10}" against "sortedCache=[{102, 102}, {101, 101}, {100, 7}]". It's obvious that "sortedCache[2]={100, 7}" is the first that cannot contain this envelope and doesn't form a tie, thus updating "sortedCache[2]={100, 10}" is a safe bet.
- At some moment of time we might have to evaluate "envelope={99, 50}" against "sortedCache=[{102, 102}, {101, 101}, {100, 10}, {99, 6}]". It's obvious that "sortedCache[2]={100, 10}" is again the first that cannot contain this envelope whilst they form a tie. As the "envelopes" are sorted w.r.t. "width", let's try "always replace upon a tie".

This simple tactic turns out incorrect, e.g. for sorted "envelopes"
```
[{16, 14}, {13, 2}, {11, 14}, {6, 10}, {6, 1}]
```
the resultant "sortedCache" will be "[{11, 14}, {6, 1}]" of length 2, where the answer should be 
```
[{16, 14}, {13, 2}, {6, 1}]
```
of length 3, yet **everything will become correct if we sort "envelopes" by descending "width" & ascending "height"**! Further information is given in the source code, note that **"sortedCache" only tells the correct length at the end of traversal but not necessarily the finally contained elements**.

# Similar problems
- [p646_Maximum_Length_of_Pair_Chain](https://github.com/genxium/Leetcode/tree/master/p646_Maximum_Length_of_Pair_Chain)
- [p1626_Best_Team_With_No_Conflicts](https://github.com/genxium/Leetcode/tree/master/p1626_Best_Team_With_No_Conflicts), more mathematical generalizations there
- [p1691_Maximum_Height_by_Stacking_Cuboids](https://github.com/genxium/Leetcode/tree/master/p1691_Maximum_Height_by_Stacking_Cuboids)
