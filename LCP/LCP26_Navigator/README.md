1. It's obvious that we should put navigators on leafs, just consider the simplest tree [1,2,3].

2. For a subtree rooted at "g" and has non-null left child "u", right child "v". It should be noted that any existing navigator outside of "subtree g" couldn't distinguish "same height nodes" inside "subtree u" and "subtree v". 

3. Similar to #2, the navigators in "subtree u" couldn't distinguish "same height nodes" in "subtree v" either.

Similar problems
- [p968_Binary_Tree_Cameras](https://github.com/genxium/Leetcode/tree/master/p968_Binary_Tree_Cameras) 
