# Bigger board version
A more difficult version of this problems will be like
```
Given the same 2 shapes of tiles (infinite supplies for both), find how many ways there're to fill a MxN board but min(M, N) <= 10, i.e. pow(2, min(M, N)) can be up to 1024. 
```

## A strawman approach
In this case, one possible approach is as follows. 
```
By "anchoredShape at (iRow, jCol)", it refers to the following alignments.

(iRow-1, jCol+0), (iRow-1, jCol+1)
(iRow+0, jCol+0), (iRow+1, jCol+1)

0. Not putting anything
This is a feasible option except when "dfs"ing the last row.
__
__

1. L shape blank at (iRow-1, jCol+1)
x_
xx  

2. L shape blank at (iRow-1, jCol+0)
_x
xx  

3. L shape blank at (iRow+0, jCol+0)
xx
_x  

4. L shape blank at (iRow+0, jCol+1)
xx
x_  

5. Bar shape vertical, bottom aligned with (iRow+0, jCol+0)
x_
x_ 

6. Bar shape horizontal, left aligned with (iRow+0, jCol+0)
__
xx

Hence each "rowState"  is a "7-ary number", e.g. [0, 1, 0, 4, 0, 5, 6]. 
```

Let's define "dp1[i][rowState]" as the number of ways to FULLY FILL rows [0, i-1], while the i-th row has "rowState". This definition might look weird at the first glance, and the answer we'd find is equal to "dp1[nRow][0]", i.e. no extrusion w.r.t. the "nRow-1"-th row. 
```python
def dfs1(i, j, nowRowState, preRowState):
    if j >= nCol:
        # We found "dp1[i-1][preRowState]" more ways to fully fill rows [0, i-1] while the i-th row has "nowRowState", guaranteed by the "isCompatible" check
        dp1[i][nowRowState] += dp1[i-1][preRowState] 
        return

    for anchoredShape in possibleAnchoredShapes:
        # That "isCompatible" check needs an "i" parameter is due to the need of checking whether we're at last row such that putting nothing is NOT an option.
        # Also note that "isCompatible" should guarantee that "preRowState" is FILLED IN PREFIX RANGE [0, j] by "(nowRowState, anchoredShape)".
        if isCompatible(i, j, preRowState, nowRowState, anchoredShape): 
            nextNowRowState = calcNewTileState(nowRowState, j, anchoredShape)
            dfs1(j+1, nextNowRowState, preRowState)    

for i in range(0,nRow+1):
    dp1[i][*] = 0;
    for preRowState where dp1[i-1][preRowState] > 0:
        dfs1(i, j=0, nowRowState=0, preRowState)

return dp1[nRow][0]
``` 

## A better approach
Notice that "dp1" can also be calculated by preprocessed knowledge "preRowStatesfilledBy[]", i.e. foreach nowRowState let `dp1[i][nowRowState] = SUM{ dp1[i-1][preRowState] for preRowState in "preRowStatesfilledBy[nowRowState]" }`, which actually forms a matrix multiplication from row to row, thus we can use "matrix [quick pow(binary divide & conquer)](https://github.com/genxium/Leetcode/tree/master/p50_Pow_x_n)" to accelerate computation.

## A better state representation
However such approach is still memory wasting. Notice that in "dfs1" each step we move from (j, nowRowState, preRowState) to (j+1, nextNowRowState, preRowState), **it's in fact a "(j, oldHybridState): (nowRowState prefix [0, j], preRowState suffix [j, nCol-1])" is moved to "(j+1, newHybridState): (nextNowRowState prefix [0, j+1], preRowState suffix [j+1, nCol-1])", where a "hybridState" has "nCol+1" columns to concern instead of "nCol" (overlay at the j-th col for doing "isCompatible" check)**.  

Is it possible to calculate a "dp2[i][j][hybridState]" instead using a "dfs2(i, j, nowHybridState)"? A definition change is proposed as follows.
```
dp2[i][j][hybridState] as the number of ways to FULLY FILL every grid STRICTLY ABOVE the polyline indicated by "(j, hybridState)" 

the answer we'd find is equal to "dp2[nRow][nCol][0]", i.e. at "nRow"-th row put nothing till the "nCol"-th col (yes the traversal extruding both rows and cols) 
```

# More complex shapes version
Another more difficult version of this problems will be like
```
Given K different shapes of tiles, e.g. an "L", a "2-bar", a "3-bar", a "star", each with infinite supplies, find how many ways there're to fill a MxN board. What about with finite supplies "tileLimit[i], 0 <= i < K"? 
```

The "infinite supply" version can be solved by converting to the [ExactCover problem](https://www.yinxiang.com/everhub/note/e71e7e87-ca06-40e5-a880-c9517a9afd33).
