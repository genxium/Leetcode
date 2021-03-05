
int const MAXN = 500;
int const INVALID = -1;
int memo[MAXN][MAXN]; // "memo[l][r]" is the min #insertions to make "s[l, ..., r]" palindromic 

class Solution {
public:
    int dfs(int l, int r, string &s) { 
        if (l >= r) return 0; 
        if (INVALID != memo[l][r]) return memo[l][r];
        
        int ret = INT_MAX;
        if (s[l] == s[r]) {
            int cand = dfs(l+1, r-1, s);
            ret = min(ret, cand);
        }
        int cand1 = 1+dfs(l+1, r, s); // insert 1 to match s[l]
        int cand2 = 1+dfs(l, r-1, s); // insert 1 to match s[r]
        
        ret = min(ret, cand1);
        ret = min(ret, cand2);
        
        return memo[l][r] = ret;
    }
    
    int minInsertions(string s) {
        memset(memo, INVALID, sizeof memo);
        return dfs(0, s.length()-1, s);
    }
};
