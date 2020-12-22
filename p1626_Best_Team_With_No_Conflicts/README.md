# Similar problems
- [p354_Russian_Doll_Envelopes](https://github.com/genxium/Leetcode/tree/master/p354_Russian_Doll_Envelopes)
- [p1691_Maximum_Height_by_Stacking_Cuboids](https://github.com/genxium/Leetcode/tree/master/p1691_Maximum_Height_by_Stacking_Cuboids)

This problem is a generalization of [p354_Russian_Doll_Envelopes](https://github.com/genxium/Leetcode/tree/master/p354_Russian_Doll_Envelopes) with weights and a looser `isCompatible(...)` condition, thus more difficult to come up with a binary-search approach.

Both [p354_Russian_Doll_Envelopes](https://github.com/genxium/Leetcode/tree/master/p354_Russian_Doll_Envelopes) and this problem make use of the `Transitive isCompatible(...) after sorting`, i.e. for "i < j < k", if "isCompatible(items[i], items[j]) && isCompatible(items[j], items[k])" then "isCompatible(items[i], items[k])" -- **yet such `Transitivity` were not true if the inputs were unsorted**.

To make the problem even more difficult, we can assume that there's no way to make the "isCompatible(...)" function "Transitive" at all, thus the problem is generalized to [Exact cover problem with weighted-rows](https://www.yinxiang.com/everhub/note/e71e7e87-ca06-40e5-a880-c9517a9afd33), where in the "ConflictGrid" we have `grid[i][j] == 1 if false==isCompatible(items[i], items[j])`.
