Like in [p1606_Find_Servers_That_Handled_Most_Number_of_Requests](https://github.com/genxium/Leetcode/tree/master/p1606_Find_Servers_That_Handled_Most_Number_of_Requests), we can easily use segtree to implement "find first element in x[] not greater than given u", but that's an overkill. This problem doesn't have a dynamic input stream thus can be traversed from rear to front with a `monotonic stack` to solve.

Similar problems
- [p853_Car_Fleet](https://github.com/genxium/Leetcode/tree/master/p853_Car_Fleet) 
- [p1606_Find_Servers_That_Handled_Most_Number_of_Requests](https://github.com/genxium/Leetcode/tree/master/p1606_Find_Servers_That_Handled_Most_Number_of_Requests)
- [p1776_Car_Fleet_II](https://github.com/genxium/Leetcode/tree/master/p1776_Car_Fleet_II) 
- [p1944_Number_of_Visible_People_in_a_Queue](https://github.com/genxium/Leetcode/tree/master/p1944_Number_of_Visible_People_in_a_Queue) 
