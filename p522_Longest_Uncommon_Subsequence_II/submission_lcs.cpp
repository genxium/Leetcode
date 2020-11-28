/*
test cases
["aba","cdc","eae"]
["aabbcc", "aabbcc","cb"]
["aaa","aaa","aa"]
["aaa","aaa","a"]
["a","a","b","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a","a"]
*/

int const MAXL = 10;
int dp[MAXL][MAXL]; // For LCS, see https://github.com/genxium/Leetcode/tree/master/p1143_Longest_Common_Subsequence for details

class StrComp {
public:
    bool operator() (string const& lhs, string const& rhs) {
        return lhs.length() > rhs.length();
    }
};

class Solution {
public:
    int longestCommonSubsequence(string &text1, string &text2) {
        int l1 = text1.length(), l2 = text2.length();
        if (l1 <= 0 || l2 <= 0) return 0;
        
        // Init.
        int maxDpVal = 0;
        
        memset(dp, 0, sizeof(dp));
        dp[0][0] = (text1[0] == text2[0] ? 1 : 0);
        maxDpVal = max(dp[0][0], maxDpVal);
        
        for (int i = 1; i < l1; ++i) {
            dp[i][0] = (text1[i] == text2[0] ? 1 : dp[i-1][0]);
            maxDpVal = max(dp[i][0], maxDpVal);
        }
        for (int j = 1; j < l2; ++j) {
            dp[0][j] = (text1[0] == text2[j] ? 1 : dp[0][j-1]);
            maxDpVal = max(dp[0][j], maxDpVal);
        }
        
        // Loop.
        for (int i = 1; i < l1; ++i) {
            for (int j = 1; j < l2; ++j) {
                int candidate = 0;
                    
                candidate = dp[i - 1][j];    
                if (dp[i][j] < candidate) dp[i][j] = candidate;
             
                candidate = dp[i][j - 1];
                if (dp[i][j] < candidate) dp[i][j] = candidate;

                if (text1[i] == text2[j]) {
                    candidate = dp[i - 1][j - 1] + 1;
                    if (dp[i][j] < candidate) dp[i][j] = candidate;
                }
                // printf("dp[%d][%d] == %d\n", i, j, dp[i][j]);
                if (dp[i][j] > maxDpVal) {
                    maxDpVal = dp[i][j];
                }
            }
        }
        return maxDpVal;
    }
  
    int findLUSlength(vector<string>& strs) {
        sort(strs.begin(), strs.end(), StrComp());
        int n = strs.size();
        unordered_map<string, int> dedupped;
        for (int i = 0; i < n; ++i) {
            bool shouldSkip = false;
            for (int j = 0; j < i; ++j) {
                if (strs[j].length() == strs[i].length()) continue;
                int lcsLength = longestCommonSubsequence(
                    strs[j], // longer
                    strs[i] // shorter
                );
                //printf("LCS length of strs[j:%d]:%s and strs[i:%d]:%s is %d\n", j, strs[j].c_str(), i, strs[i].c_str(), lcsLength);
                if (lcsLength == (int)(strs[i].length())) {
                    shouldSkip = true;
                    break;
                }
            }
            if (!shouldSkip) {
                ++dedupped[strs[i]];                
            }
        }

        int ans = -1;
        for (auto &it : dedupped) {
            string s = it.first;
            int count = it.second;
            if (count > 1) continue;
            int sLen = s.length();
            //printf("checking s: %s, count: %d, sLen: %d, ans: %d\n", s.c_str(), count, sLen, ans);
            if (sLen > ans) ans = sLen;
        }

        return ans;
    }
};
