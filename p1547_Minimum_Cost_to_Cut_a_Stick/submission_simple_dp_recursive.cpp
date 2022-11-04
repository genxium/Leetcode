bool const debug = false;
int const MAXCUTS = 100;
int const INVALID = -1;
int dp[MAXCUTS+2][MAXCUTS+2]; // "dp[l][r]" is the minimum cost to perform all cuts within (cuts[l], cuts[r])

int dfs(int l, int r, vector<int>& cuts) {
    if (INVALID != dp[l][r]) return dp[l][r];
    if (l+1 >= r) return dp[l][r] = 0; // monopiece couldn't be cut
    int result = INT_MAX;
    for (int k = l+1; k < r; ++k) {
        int t = cuts[r]-cuts[l];
        int cand = t + dfs(l, k, cuts) + dfs(k, r, cuts);
        if (cand >= result) continue;
        result = cand;
    }
    return dp[l][r] = result;
}

/*
test cases
36
[5,6,8,11,13,15]
36
[13,17,15,18,3,22,27,6,35,7,11,28,26,20,4,5,21,10,8]
*/
class Solution {
public:
    int minCost(int n, vector<int>& cuts) {
        sort(cuts.begin(), cuts.end());
        cuts.insert(cuts.begin(), 0);
        cuts.push_back(n);
      
        // init        
        memset(dp, INVALID, sizeof(dp));
        
        return dfs(0, cuts.size()-1, cuts);
    }
};

