Though possible to have nested children, the solution to this problem DOESN'T require a nested "localCounter" like that of [p726_Number_of_Atoms](https://github.com/genxium/Leetcode/tree/master/p726_Number_of_Atoms), because there is no information to pass from a child to the parent.

In case some extracted information, e.g. a "localMap/localCounter", from a child should be passed to a parent recursively, the triggers are as follows
- push a new "localMap/localCounter" upon transition `EXPECTING_TAG_EMPTY_MIXED -> EXPECTING_TAG_NAME_MORE`  
- pop the current "localMap/localCounter" upon transition `EXPECTING_TAG_END_NAME -> EXPECTING_TAG_CONTENT` and update the "localMap/localCounter of parent".

Such pushing & popping can be as well implemented iteratively using a similar `stack<pair<string, unordered_map<..., ...>>> stkActiveTags`, see [p636_Exclusive_Time_of_Functions](https://github.com/genxium/Leetcode/tree/master/p636_Exclusive_Time_of_Functions) for details. 

Similar problems
- [p722_Remove_Comments](https://github.com/genxium/Leetcode/tree/master/p722_Remove_Comments)
