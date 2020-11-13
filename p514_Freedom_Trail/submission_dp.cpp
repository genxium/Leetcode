int const INVALID = -1;
int const MAXKEYLENGTH = 100;
int const MAXRINGLENGTH = 100;

int dp[MAXKEYLENGTH][MAXRINGLENGTH]; // dp[i][j] == x where ring[j] == keys[i], means that there're at least x steps needed to match keys[i] with the "click step" stops at ring[j]

/*
test case#1
"godding"
"gd"

test case#2
"godding"
"gdidg"

test case#3
"godding"
"gdidgdinggi"
*/
class Solution {
public:
  int findRotateSteps(string ring, string key) {
    unordered_map<char, unordered_set<int>> keyToRingCounterclockwiseOffsets;

    // init
    int n = ring.size(), m = key.size();

    for (auto ch : key) {
      for (int j = 0; j < n; ++j) {
        if (ch != ring[j]) continue; 
        keyToRingCounterclockwiseOffsets[ch].insert(j);
      }
    }

    memset(dp, 0, sizeof(dp));
    for (auto j : keyToRingCounterclockwiseOffsets[key[0]]) {
      dp[0][j] = min(j, n-j) + 1; // rotate "j" steps counterclockwisely or "n-j" steps clockwisely, then "press"
      //printf("dp[0][j:%d] == %d\n", j, dp[0][j]);
    }

    // loop
    for (int i = 1; i < m; ++i) {
      for (auto j : keyToRingCounterclockwiseOffsets[key[i]]) {
        int tmp = INT_MAX;
        for (auto t : keyToRingCounterclockwiseOffsets[key[i-1]]) {
          int candidate = 1 + min((j-t+n)%n, (t-j+n)%n) + dp[i-1][t];
          tmp = min(tmp, candidate);
        }
        dp[i][j] = tmp;
        //printf("dp[i:%d][j:%d] == %d\n", i, j, dp[i][j]);
      }
    }

    int ans = INT_MAX;
    for (auto j : keyToRingCounterclockwiseOffsets[key[m-1]]) {
      if (INVALID == dp[m-1][j]) continue;
      ans = min(ans, dp[m-1][j]);
    }
    
    return ans;
  }
};
