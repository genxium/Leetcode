Though having a classical "backtracking" characteristic(like [p51_N-queens](https://leetcode.com/problems/n-queens/) or [p37_Sudoku_Solver](https://github.com/genxium/Leetcode/tree/master/p37_Sudoku_Solver)), this problem can be solved either recursively or iteratively. 

# Recursive
The recursive solution is pretty straight-forward.

# Iterative
Consider the following grid.
```
1abc
gfed
hi2*
```
where '\*' is an obstacle, 'a' is an empty grid named 'a', same nomenclature goes with 'b', 'c', 'd', ... and so on.

The followings are some valid unique paths.
- [1, a, b, c, d, e, f, g, h, i, 2]
- [1, g, h, i, f, a, b, c, d, e, 2]

Notice that the orders of 'a' and 'g' in the paths above are inverted, making it non-trivial to just iterate over the grid in a certain direction for accumulating counts.  

# Reference

Here's [another problem(p10_Regular_Expression_Match)](https://github.com/genxium/Leetcode/tree/master/p10_Regular_Expression_Matching) which can as well be solved either recursively or iteratively, yet without the "backtracking" characteristic. 
