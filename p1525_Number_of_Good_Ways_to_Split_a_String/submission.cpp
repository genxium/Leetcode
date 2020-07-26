class Solution {
public:
    unordered_map<char, int> subtract(unordered_map<char, int> &lhs, unordered_map<char, int> &rhs) {
      unordered_map<char, int> toRet;
      for (auto it = lhs.begin(); it != lhs.end(); ++it) {
        char c = it->first;
        toRet[c] = (lhs[c] - rhs[c]);
        if (toRet[c] == 0) {
          toRet.erase(c);
        }
      }
      return toRet;
    }
  
    bool sameDistinctKeyCount(unordered_map<char, int> &lhs, unordered_map<char, int> &rhs) {
      int lhsKeyCount = 0;
      for (auto it = lhs.begin(); it != lhs.end(); ++it) {
        if (it->second > 0) {
          ++lhsKeyCount;
        }
      }
      int rhsKeyCount = 0;
      for (auto it = rhs.begin(); it != rhs.end(); ++it) {
        if (it->second > 0) {
          ++rhsKeyCount;
        }
      }
      return lhsKeyCount == rhsKeyCount;
    }
  
    int numSplits(string s) {
      unordered_map<char, int> wholeCounter, prefixCounter;
      int ans = 0;
      for (auto &c : s) {
        ++wholeCounter[c];
      }
      
      int n = s.length();
      unordered_map<char, int> suffixCounter = subtract(wholeCounter, prefixCounter);

      for (int i = 0; i < n-1; ++i) {
        // the split makes [i+1, n) the other segment
        char c = s[i];
        ++prefixCounter[c];
        --suffixCounter[c];
        
        if (sameDistinctKeyCount(prefixCounter, suffixCounter)) {
          ++ans;
        }
      }
      return ans;
    }
};
