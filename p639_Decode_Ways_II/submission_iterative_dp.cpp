bool const debug = false;
int const MAXN = 100000;
int const NOT_PAIRED_WITH_PREV = 0;
int const PAIRED_WITH_PREV = 1;

typedef uint64_t ULL;
ULL const MOD = 1000000007;
    
/*
That "dp[i][NOT_PAIRED_WITH_PREV | PAIRED_WITH_PREV] == x" means that there're "x" ways to decode "s[0, ..., i]" if "s[i] is unpaired/paired with s[i-1], respectively".
*/
ULL dp[MAXN][2]; 

/*
test cases
"*"
"**"
"1*"
"1*1"
"1**"
"*3*"
"2*3*"
"22*3*"
"22*3*1*"
"22*3**1*"
"9*****1*0*12*16*8*0000"
"9*****1*0*12*16*8*0"
*/
class Solution {
public:
    int singleDecodeWays(char ch) {
        if (ch == '0') return 0;
        return (ch == '*' ? 9 : 1);
    }
    
    int pairDecodeWays(char lhs, char rhs) {
        if (lhs == '*') {
            if (rhs != '*') {
                if (rhs < '7') return 2; // lhs == '1' or lhs == '2'
                else return 1; // lhs == '1'
            } else {
                return 15; // "11, 12, 13, .., 19, 21, 22, 23, 24, 25, 26", no "10", no "20"
            }
        }
        
        // Now that lhs is not '*', i.e. is digit 
        if (lhs == '0' || lhs > '2') return 0;
        if (lhs == '1') {
            if (rhs != '*') return 1;
            else return 9;
        } else {
            // lhs == '2'
            if (rhs != '*') return (rhs < '7' ? 1 : 0);
            else return 6;
        }
    }
    
    int numDecodings(string s) {
        memset(dp, 0, sizeof dp);
        int n = s.length();
        
        // init
        dp[0][NOT_PAIRED_WITH_PREV] = singleDecodeWays(s[0]);
            
        // loop
        for (int i = 1; i < n; ++i) {
            ULL &t1 = dp[i][NOT_PAIRED_WITH_PREV];
            if (singleDecodeWays(s[i]) > 0) {
                t1 += singleDecodeWays(s[i])*dp[i-1][NOT_PAIRED_WITH_PREV];
                t1 %= MOD;
                t1 += singleDecodeWays(s[i])*dp[i-1][PAIRED_WITH_PREV];
                t1 %= MOD;
            }  
            
            ULL &t2 = dp[i][PAIRED_WITH_PREV];
            if (pairDecodeWays(s[i-1], s[i]) > 0) {
                if (i-2 < 0) {
                    t2 += pairDecodeWays(s[i-1], s[i]);
                    t2 %= MOD;    
                } else { 
                    t2 += pairDecodeWays(s[i-1], s[i])*dp[i-2][NOT_PAIRED_WITH_PREV]; 
                    t2 %= MOD;
                    t2 += pairDecodeWays(s[i-1], s[i])*dp[i-2][PAIRED_WITH_PREV];
                    t2 %= MOD;
                }   
            }
        }
        int ans = (dp[n-1][NOT_PAIRED_WITH_PREV] + dp[n-1][PAIRED_WITH_PREV]) % MOD;
        return ans;
    }
};
