bool debug = false;

typedef uint64_t ULL;
ULL const MOD = 1000000007;
int const MAXN = 2000;
int const INVALID = -1;

ULL dp[MAXN]; // The number of UNIQUE CONTENT subseqs of "S[0, ..., i]", NOT NECESSARILY using "S[i]"
int lastPos[26]; // Position of the last occurrence of character

/*
test cases
"abc"
"aba"
"aaa"
"lee"
"leee"
"efdadd"
*/
class Solution {
public:
    int distinctSubseqII(string S) {
        memset(dp, 0, sizeof dp);
        memset(lastPos, INVALID, sizeof lastPos);

        // Init
        dp[0] = 2; // {"", S[0]}
        lastPos[S[0]-'a'] = 0;

        // Loop
        for (int i = 1; i < S.length(); ++i) {
            dp[i] = (dp[i-1]*2)%MOD; // Append "S[i]" to all UNIQUE CONTENT subseqs counted by "dp[i-1]"
            int chIdx = S[i]-'a';
            if (INVALID != lastPos[chIdx]) {
                /*
                Arbitrarily assume that S[i] == 'x', thus S[lastPos['x']] == 'x' by definition. Appending 'x' to each UNIQUE CONTENT subseqs counted by "dp[lastPos[chIdx]-1]" makes a collection "C1". All strings in "C1" are unique to each other, yet "C1" is already counted by "dp[lastPos[chIdx]]", therefore duplicate w.r.t. "dp[i-1]".  
                */
                ULL duplicates = lastPos[chIdx]-1 >= 0 ? dp[lastPos[chIdx]-1] : 1; // If "0 == lastPos[chIdx]", only 1 single letter string is duplicated.
                dp[i] = (dp[i]-duplicates+MOD)%MOD;
            }
            lastPos[chIdx] = i;
        }
        
        ULL ans = (dp[S.length()-1]-1+MOD)%MOD; // Remove the empty set 
        return ans;
    }
};
