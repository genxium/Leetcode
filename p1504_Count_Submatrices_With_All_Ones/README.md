In the "naive_dp" solution, there's room for improvement. 

Denote that `t[y][x]` as the topmost y-position that `[y][x]` can consecutively reach all `1`s(y+ is `down`, x+ is `right`), specifically 
- `t[y][x] = INVALID` if `0 == mat[y][x]` and 
- `t[y][x] = y` if `1 == mat[y][x] && 0 == mat[y-1][x]`. 

Note that to calculate each `dp[y][x][h]` we only care about whether `h` is included in `c = {dp[y][x-1][0], dp[y][x-1][1], ..., dp[y][x-1][h']}`, where 

- `c` should be initialized to empty upon `y` updates, and
- `c` should be initialized to empty upon `0 == mat[y][x]`, and
- `c` should support `c.getMaxH()`, and 
- `c` should support `c.sum()`, and 
- `c` should support `c.upsert(h'' -> dp[y][x-1][h''])`, and 
- `c` should support `c.del(h'')` 

for iterating by `[y][x]`. 
