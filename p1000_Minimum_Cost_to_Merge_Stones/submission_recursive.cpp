bool debug = false;
int const INVALID = -1;
int const MAXN = 30;
int dp[MAXN][MAXN][MAXN]; // dp[l][r][c] is the min-cost of applying "c" steps on "stones[l, ..., r]"

/*
test cases
[3,2,4,1]
2
[3,2,4,1]
3
[3,5,1,2,6]
3
[4,6,4,7,5]
2
*/
class Solution {
public:
    int dfs(int l, int r, int c, int K, vector<int> &ps /* prefixSum */) {
        int len = r-l+1; 
        if (len < 1+c*(K-1)) return dp[l][r][c] = INT_MAX; // couldn't make "c" cuts on length "len"
        if (INVALID != dp[l][r][c]) return dp[l][r][c];
        if (0 == c) return dp[l][r][c] = 0;
        if (1 == c) {
            int ret = INT_MAX;
            for (int i = l; i+K-1 <= r; ++i) ret = min(ret, ps[i+K]-ps[i]);
            if (debug && INT_MAX != ret) printf("dp[l:%d][r:%d][c:%d] == %d\n", l, r, c, ret);
            return dp[l][r][c] = ret;
        }
        
        int ret = INT_MAX;
        bool cutToOne = len == 1+c*(K-1);
        // It's noticeable that when "true == cutToOne", we DON'T care about "what stone indices are remained from the left cuts or the right cuts", the "finalizedCutCost" is always "ps[r+1]-ps[l]"
        int finalizedCutCost = (cutToOne ?  ps[r+1]-ps[l] : 0);
        // Consider each "stones[l, ..., i]" and "stones[i+1, ..., r]" to share the total "c-cutToOne" steps
        for (int i = l; i+1 <= r; ++i) {
            int leftLen = i-l+1;
            int leftCUpper = min(leftLen/(K-1), c-cutToOne);
            for (int leftC = 0; leftC <= leftCUpper; ++leftC) {
                int rightC = c-cutToOne-leftC;
                int leftCand = dfs(l, i, leftC, K, ps);
                if (INT_MAX == leftCand) continue;
                int rightCand = dfs(i+1, r, rightC, K, ps);
                if (INT_MAX == rightCand) continue;
                int cand = leftCand+rightCand+finalizedCutCost;
                ret = min(ret, cand);
            }
        }
        
        if (debug && INT_MAX != ret) printf("dp[l:%d][r:%d][c:%d] == %d\n", l, r, c, ret);
        return dp[l][r][c] = ret;
    }
    
    int mergeStones(vector<int>& stones, int K) {
        memset(dp, INVALID, sizeof dp);   
        vector<int> ps = {0};
        for (auto &stone : stones) ps.push_back(ps.back() + stone);
        
        int n = stones.size();
        // Each merge reduces the length by "K-1".
        if ((n-1) % (K-1) != 0) return INVALID;
        
        int c = (n-1)/(K-1);
            
        int ans = dfs(0, n-1, c, K, ps); 
        return ans;
    }
};
