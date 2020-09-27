class Solution {
public:
    int solve(string &s, int stClosed, int edOpen, int k) {
      if (stClosed >= edOpen) return 0;
      
      unordered_map<char, int> counter;
      for (int i = stClosed; i < edOpen; ++i) {
        char ch = s[i];
        ++counter[ch];
      }
      
      int toRet = 0;
      bool allLargerThanOrEqualToK = true;
      for (auto &it : counter) {
        if (it.second < k) allLargerThanOrEqualToK = false;
      }
      
      if (allLargerThanOrEqualToK) return edOpen-stClosed;
      
      int lastStart = stClosed;
      for (int i = stClosed; i < edOpen; ++i) {
        if (counter[s[i]] < k) {
          // For such 's[i]', we just cannot include ANY of it in the resultant substring, i.e. the resultant substring MUST reside either on the left side or the right side of 's[i]'.
          toRet = max(toRet, solve(s, lastStart, i, k));
          lastStart = i+1;
        }
      }
      
      // wrap up
      toRet = max(toRet, solve(s, lastStart, edOpen, k));
      
      return toRet;
    }
  
    int longestSubstring(string s, int k) {
      return solve(s, 0, s.length(), k);
    }
};
