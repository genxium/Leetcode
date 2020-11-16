int const INVALID = -1;
int const MAXCOLOR = 100;
int const MAXN = 100;
typedef pair<int, int> PII;

PII dp[MAXN][MAXN][MAXCOLOR+1]; // That "dp[l][r][c] = {x, t}" means by bursting "boxes[l, ..., r]" exact for those of color "c", we can get at most "x" points and "t" count of color "c" boxes", in case there couldn't be any color "c" box remained by bursting "boxes[l, ..., r]" assign "t = 0".

/*
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

test case#9
[5,5,3,9,2,4,4,6,5,8,4,8,6,9,6,2,8,6,4,1,9,5,3,10,5,3,3,9,8,8,6,5,3,7,4,9,6,3,9,4,3,5,10,7]

test case#10
[5,8,4,8,6,9,6,2,8,6,4,1,9,5,3,5,3,3,9,8,8,6,5]

test case#11
[5,8,8,6,9,8,6,9,5,8,6,5]

test case#12
[1,2]

test case#13
[39,57,64,26,5,11,38,96,62,21,99,80,2,64,2,71,54,68,30,52,63,53,87,31,87,79,22,77,49,54,49,37,60,55,87,53,20,63,76,64,91,14,91,8,3,95,98,10,47,98,43,7,41,9,15,59,43,86,16,10,45,67,37,4,62,90,87,41,20,74,66,30,62,12,18,89,63,82,85,26,89,19,40,54,70,50,60,44,83,59,65,21,68,70,47,13,25,3,69,4]

test case#14
[4,8,3,10,10,8,3,6,3,3,4,9,6,1,4,8,2,3,1,10,6,2,2,4,9,3,6,4,6,5,3,9,9,10,10,6,2,8,3,2,3,10,5,1,8,2,4,9,7,2]
*/
class Solution {
private:
    void printDp(int l, int r, int c) {
      PII &val = dp[l][r][c];
      printf("dp[l:%d][r:%d][c:%d] == {x: %d, t: %d}\n", l, r, c, val.first, val.second);
    }
    
    void updateDpIfApplicable(PII &dpVal, int candidateX, int candidateT) { 
      // if (candidateX <= dpVal.first || candidateT <= dpVal.second), a GREEDY ASSUMPTION is introduced below
      if (candidateX + candidateT*candidateT > dpVal.first + dpVal.second*dpVal.second) {
        dpVal.first = candidateX;
        dpVal.second = candidateT;
      }
    }
public:
    int removeBoxes(vector<int>& boxes) {
      int n = boxes.size();
      // init
      for (int l = 0; l < n; ++l) {
        for (int r = 0; r < n; ++r) {
          for (int c = 1; c <= MAXCOLOR; ++c) {
            dp[l][r][c] = {0, 0};
          }
        }
      }

      for (int l = 0; l < n; ++l) {
        for (int c = 1; c <= MAXCOLOR; ++c) {
          if (c == boxes[l]) {
            dp[l][l][c] = {0, 1};                
          } else {
            dp[l][l][c] = {1, 0}; // bursts "boxes[l]" to obtain "x == 1"
          }
        }
      }

      // loop
      for (int len = 2; len <= n; ++len) {
        for (int l = 0; l+len-1 < n; ++l) {
          int r = l+len-1;
          int maxPoints = 0;
          for (int c = 1; c <= MAXCOLOR; ++c) {
            for (int k = l; k < r; ++k) {
              int candidateX = dp[l][k][c].first + dp[k+1][r][c].first;
              int candidateT = dp[l][k][c].second + dp[k+1][r][c].second;
              updateDpIfApplicable(dp[l][r][c], candidateX, candidateT);
              maxPoints = max(maxPoints, dp[l][r][c].first + dp[l][r][c].second*dp[l][r][c].second);
            }
          }
          
          for (int c = 1; c <= MAXCOLOR; ++c) {
            if (0 == dp[l][r][c].second || dp[l][r][c].first + dp[l][r][c].second*dp[l][r][c].second < maxPoints) {
              dp[l][r][c].first = maxPoints;
              dp[l][r][c].second = 0;
            }
          }
        }
      }
      
      int ans = 0;
      for (int c = 1; c <= MAXCOLOR; ++c) {
        int candidate = dp[0][n-1][c].first + dp[0][n-1][c].second*dp[0][n-1][c].second;
        ans = max(ans, candidate);
      }

      return ans;
  }
};
