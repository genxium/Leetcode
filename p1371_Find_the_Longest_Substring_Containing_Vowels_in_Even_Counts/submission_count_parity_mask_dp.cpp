#define INVALID -1
#define MAX_SUM_MASK 32 // (1 << 5)

int digitOf(char ch) {
  switch(ch) {
    case 'a':
      return 0;
    case 'e':
      return 1;
    case 'i':
      return 2;
    case 'o':
      return 3;
    case 'u':
      return 4;
  }
  return INVALID;
}

// for each digit "`ch` in [a, e, i, o, u]", a "(sumMask & (1 << digitOf(ch))) > 0" means that there's an "odd count of `ch`" for the interval that produces "sumMask"
// "dp[sumMask] == i" means that the earliest occurrence of "sumMask" was at s[i], i.e. for s[0, ..., i]
int dp[MAX_SUM_MASK]; 

class Solution {
public:
    int findTheLongestSubstring(string s) {
        memset(dp, INVALID, sizeof(dp));
        int l = s.length();
        
        // init
        int ans = 0;
        int sumMaskByFar = 0;
        dp[sumMaskByFar] = INVALID; // it's obvious that even for "s[0, ..., 0]", sumMaskByFar couldn't be 0(i.e. all counts even)
        
        // loop
        for (int i = 0; i < l; ++i) {
            int dCh = digitOf(s[i]);
            if (INVALID != dCh) {
                sumMaskByFar ^= (1 << dCh);  
            }
            int prevEarliestOccurrence = dp[sumMaskByFar]; 
            if (INVALID != prevEarliestOccurrence || 0 == sumMaskByFar) {
                ans = max(ans, i - prevEarliestOccurrence);
            } else {
                dp[sumMaskByFar] = i;
            }
        }
        return ans;
    }
};

