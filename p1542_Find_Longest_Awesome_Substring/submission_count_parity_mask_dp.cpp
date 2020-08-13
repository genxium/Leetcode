#define INVALID -1
#define MAX_SUM_MASK 1024

// for each digit "d in [0, 9]", a "(sumMask & (1 << d)) > 0" means that there's an "odd count of `d`" for the interval that produces "sumMask"
// "dp[sumMask] == i" means that the earliest occurrence of "sumMask" was at s[i], i.e. for s[0, ..., i]
int dp[MAX_SUM_MASK]; 

class Solution {
public:
    int longestAwesome(string s) {
        /*
        A substring s[i, ..., j] is "awesome" iff
        - when (j-i+1) is even, the corresponding "sumMask" is all '0'
        - when (j-i+1) is odd, the corresponding "sumMask" contains only one '1'
        */
        
        /*
        test case #1
        "3242415"
        
        test case #2
        "373781"
        */
        memset(dp, INVALID, sizeof(dp));
        int l = s.length();
        
        // init
        int ans = 0;
        int sumMaskByFar = 0;
        dp[sumMaskByFar] = INVALID; // it's obvious that even for "s[0, ..., 0]", sumMaskByFar couldn't be 0(i.e. all counts even)
        
        // loop
        for (int i = 0; i < l; ++i) {
            int d = (s[i] - '0');
            sumMaskByFar ^= (1 << d);
            int prevEarliestOccurrence = dp[sumMaskByFar]; 
            if (INVALID != prevEarliestOccurrence || 0 == sumMaskByFar) {
                // it's implied that (i - prevEarliestOccurrence) being certainly even here
                ans = max(ans, i - prevEarliestOccurrence);
            } else {
                dp[sumMaskByFar] = i;
            }
            for (int selectedD = 0; selectedD <= 9; ++selectedD) {
                // consider that for only 1 digit "selectedD", the "parity of count of `selectedD`" differs
                int candidateSumMask = (sumMaskByFar ^ (1 << selectedD)); 
                if (INVALID != dp[candidateSumMask] || 0 == candidateSumMask) {
                    // it's implied that (i - dp[candidateSumMask]) being certainly odd here
                    ans = max(ans, i - dp[candidateSumMask]);
                }
            }
        }
        return ans;
    }
};
