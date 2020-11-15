int const INVALID = -1;
int const MAXCOLOR = 100;
int const MAXN = 100;

int cache[MAXN+1][MAXN+1][MAXN+1]; // "cache[l][r][k] == x" means that by having "k" preceding boxes of the same color as "boxes[l]"", we can burst "boxes[l, ..., r]" and obtain at most "x" points

/*
test case#1
[1,3,2,2,2,3,4,3,1]

test case#2
[3,3,3,2,2,2,3,3,3,2,2,2]

test case#3
[3,3,3,2,2,3,3,3,2,2]

test case#4
[3,3,2,2,3,3,2,1,99,99,98,99,99,97,97,97,97,97,97,97,97,97,2,97,97,97,97]

test case#5
[3,3,3,2,2,3,3,3,2,1,99,99,98,99,99,99,99,97,97,97,97,97,97,97,97,97,2,97,97,97,97,97,97,97]

test case#6
[3,8,8,5,5,3,9,2,4,4,6,5,8,4,8,6,9,6,2,8,6,4,1,9,5,3,10,5,3,3,9,8,8,6,5,3,7,4,9,6,3,9,4,3,5,10,7,6,10,7]

test case#7
[5,6,4,5,1,4,8,2,3,4,8,1,9,7,8,8,4,2,10,2,5,1,3,4,8,6,9,2,4,10,4,9,3,7,1,3,1,4,10,9,2,4,2,6,2,1,8,4,5,5]

test case#8
[86,26,80,27,1,16,78,71,36,52,65,76,58,77,45,17,100,37,37,75,49,2,37,42,19,99,14,33,34,58,4,30,100,88,74,47,80,77,85,32,80,35,80,25,60,91,99,27,47,66,13,20,15,10,26,39,60,9,63,24,66,32,29,79,67,19,88,35,44,67,22,99,27,27,40,78,2,21,40,69,88,26,57,23,15,70,1,100,37,20,26,18,27,86,88,33,28,40,92,15]
*/
class Solution {
public:
  int solve(int l, int r, int k, vector<int>& boxes) {
    if (INVALID != cache[l][r][k]) return cache[l][r][k];
    // printf("solve(l: %d, r: %d, k: %d)\n", l, r, k);
    if (l > r) return cache[l][r][k] = 0;
    
    int p = l, accK = k;
    while (p <= r && boxes[p] == boxes[l]) {
      ++p;
      ++accK;
    }
    // now that either "p == r+1" or "boxes[p] != boxes[l] but boxes[p-1] == boxes[l]" 
    
    int toRet = accK*accK + solve(p, r, 0, boxes); // bursts "boxes[l, ..., p-1]", and then bursts "boxes[p, ..., r]"
    for (int q = p; q <= r; ++q) {
      if (boxes[l] != boxes[q]) continue;
      // now that "boxes[l] == boxes[q]"
      int candidate = 
        solve(p, q-1, 0, boxes) // doesn't burst "boxes[l, p-1]" but bursts "boxes[p, q-1]"
        +
        solve(q, r, accK, boxes); // then bursts "boxes[q, r]"
      toRet = max(toRet, candidate);
    }
    
    return cache[l][r][k] = toRet;
  }
  
  int removeBoxes(vector<int>& boxes) {
    int n = boxes.size();
    memset(cache, INVALID, sizeof(cache));
    int ans = solve(0, n-1, 0, boxes);
    return ans;
  }
};
