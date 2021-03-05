bool const debug = false;
int const MAXN = 100;
int const INVALID = -1;
int memo1[MAXN][MAXN]; // "memo1[i][c]" is the min #changes to make "s[i, ...]" be composed of exactly "c" palindromes
int memo2[MAXN][MAXN]; // "memo2[l][r]" is the #changes to make "s[l, ..., r]" a single palindrome

int changesToMakePalindromic(string &s, int l, int r) {
    if (INVALID != memo2[l][r]) return memo2[l][r];
    if (l >= r) {
        memo2[l][r] = 0;
    } else {
        memo2[l][r] = (s[l] == s[r] ? 0 : 1) + changesToMakePalindromic(s, l+1, r-1);
    }
    if (debug) printf("changesToMakePalindromic(l:%d, r:%d) == %d\n", l, r, memo2[l][r]);
    return memo2[l][r];
}

class Solution {
public:
    int dfs(string &s, int i, int c) {
        if (debug) printf("dfs c:%d, i:%d\n", c, i);
        if (i == s.length()) {
            if (c == 0) return 0;
            else return INT_MAX;
        }
        
        if (c == 0) {
            if (i == s.length()) return 0;
            else return INT_MAX; // We must use the whole of "s" in this problem
        }
        
        if (INVALID != memo1[i][c]) return memo1[i][c];
        if (c == 1) {
            memo1[i][c] = changesToMakePalindromic(s, i, s.length()-1);
        } else {
            memo1[i][c] = INT_MAX;
            for (int j = i+1; j < s.length(); ++j) {
                // break into s[i, j-1] and s[j, s.length()]
                int cand1 = changesToMakePalindromic(s, i, j-1);
                int cand2 = dfs(s, j, c-1);
                if (INT_MAX == cand2) continue;

                memo1[i][c] = min(memo1[i][c], cand1+cand2);
            }   
        }
        return memo1[i][c];
    }
    
    int palindromePartition(string s, int k) {
        memset(memo1, INVALID, sizeof memo1);
        memset(memo2, INVALID, sizeof memo2);
        
        return dfs(s, 0, k);
    }
};
