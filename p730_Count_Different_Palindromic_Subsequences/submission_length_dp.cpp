typedef int64_t LL;
LL const MOD = 1000000007;
int const MAXN = 1000;
int const MAXCH = 4;

class Solution {
public:
    int solve1(string& S) {
        int n = S.length();
        
        /*
        "dp[l][r][ch]" is the number of "PalindromicSubsequence"s with distinct content and bordered by "ch"
        
        dp[l][r][ch] = {
            if (l == r && S[l] != ch) 0,
            if (l == r && S[l] == ch) 1,
            if (S[l] != ch || S[r] != ch) dp[l][r-1][ch] + dp[l+1][r][ch] - dp[l+1][r-1][ch],
            if (S[l] == S[r] == ch) SUM(dp[l+1][r-1][anyCh]) + 2 // The 1st term is "extended by border", while the 2nd term is "border itself for length 1 and length 2".
        }
        */
        LL dp[MAXN][MAXN][MAXCH]; 
        memset(dp, 0, sizeof dp);
        
        for (int len = 1; len <= n; ++len) {
            int lUpper = n-len+1;
            for (int l = 0; l < lUpper; ++l) {
                int r = l+len-1;
                for (int chIdx = 0; chIdx < MAXCH; ++chIdx) {
                    char ch = 'a'+chIdx;
                    if (l == r) {
                        dp[l][r][chIdx] = (S[l] == ch ? 1 : 0);
                    } else if (S[l] == ch && S[r] == ch) {
                        dp[l][r][chIdx] = 2;
                        for (int innerChIdx = 0; innerChIdx < MAXCH; ++innerChIdx) {
                            dp[l][r][chIdx] += dp[l+1][r-1][innerChIdx];
                            dp[l][r][chIdx] %= MOD;
                        }
                    } else {
                        // (S[l] != ch || S[r] != ch) 
                        dp[l][r][chIdx] = dp[l+1][r][chIdx] + dp[l][r-1][chIdx] - dp[l+1][r-1][chIdx];
                        dp[l][r][chIdx] %= MOD;
                    }
                }
            }    
        }
        
        int ans = 0;
        for (int chIdx = 0; chIdx < MAXCH; ++chIdx) {
            ans += dp[0][n-1][chIdx];
            ans %= MOD;
        }
        
        return ans;
    }
    
    int solve2(string &S) {
        int n = S.length();
        /*
        This solution is a slower variant of "solve1(...)" when "n" is larger than "MAXCH*MAXCH", yet having an advantage on that it doesn't rely on the fact that "ch" is limited to a small CHARSET.
        */
        LL dp[MAXN][MAXN];
        memset(dp, 0, sizeof dp);
        
        for (int len = 1; len <= n; ++len) {
            int lUpper = n-len+1;
            for (int l = 0; l < lUpper; ++l) {
                int r = l+len-1;
                if (l == r) {
                    dp[l][r] = 1;
                } else {
                    int lChIdx = S[l]-'a', rChIdx = S[r]-'a';
                    if (lChIdx != rChIdx) {
                        dp[l][r] = (dp[l+1][r] + dp[l][r-1])%MOD - dp[l+1][r-1] + MOD;
                        dp[l][r] %= MOD;
                    } else {
                        int chIdx = lChIdx;
                        char ch = 'a'+chIdx;
                        int firstOccurrence = l+1, lastOccurrence = r-1;
                        while (firstOccurrence <= lastOccurrence && S[firstOccurrence] != ch) ++firstOccurrence;
                        while (lastOccurrence >= firstOccurrence && S[lastOccurrence] != ch) --lastOccurrence;
                        if (firstOccurrence > lastOccurrence) {
                            // No "ch" within S[l+1, ..., r-1].
                            dp[l][r] = (dp[l+1][r-1] // those not extended at all 
                                     + dp[l+1][r-1])%MOD // those extended by "ch"
                                     + 2; // "border itself for length 1 & length 2"
                        } else if (firstOccurrence == lastOccurrence) {
                            // Only 1 occurrence of "ch" within S[l+1, ..., r-1], thus "border itself for length 1" is already accounted.
                            dp[l][r] = (dp[l+1][r-1] // those not extended at all 
                                     + dp[l+1][r-1])%MOD // those extended by "ch"
                                     + 1; // "border itself for length 2 only"
                        } else {
                            // firstOccurrence < lastOccurrence
                            // There're 2 or more occurrences of "ch" within S[l+1, ..., r-1], thus "border itself for length 1 & length 2" are both already accounted.
                            dp[l][r] = (dp[l+1][r-1] // those not extended at all 
                                     + dp[l+1][r-1])%MOD // those extended by "ch"
                                     - dp[firstOccurrence+1][lastOccurrence-1] // those counted twice as `extended by "ch"`
                                    + MOD;
                        }
                        
                        dp[l][r] %= MOD;
                    }
                }
            }    
        }
        
        return dp[0][n-1];
    }
    
    int countPalindromicSubsequences(string S) {
        /*
        A easier problem would be counting the number of "PalindromicSubsequence"s which are NOT NECESSARILY DIFFERENT IN CONTENT.
        
        dpMightBeSameContent[l][r] = {
            if (l == r) 1,
            if (S[l] != S[r]) dpMightBeSameContent[l][r-1] + dpMightBeSameContent[l+1][r] - dpMightBeSameContent[l+1][r-1], 
            if (S[l] == S[r]) (dpMightBeSameContent[l][r-1] + dpMightBeSameContent[l+1][r] - dpMightBeSameContent[l+1][r-1]) + (dpMightBeSameContent[l+1][r-1]) + 1 // The 2nd term is "extended by border", while the 3rd term is "border itself for length 2" -- also note that in this case "border itself for length 1" is counted twice, one in "dpMightBeSameContent[l][r-1]" & one in "dpMightBeSameContent[l+1][r]".
        }
        */ 
        // return solve2(S);
        return solve1(S);
    }
};
