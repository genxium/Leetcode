/*
test case#1
"a"

test case#2
"cac"

test case#3
"zab"

test case#4
"abcdefghixyzabcdefg"
*/
class Solution {
public:
    void count(unordered_map<char, unordered_map<int, bool>> &cache, pair<char, int> &tuple, int &ans) {
      /*
      The substring in "s" that starts with character 'ch' and extends to length 'r' definitely contains some shorter substrings, and the key to this problem is how to quickly account those shorter substrings.
      */
      
      char ch = tuple.first;
      int r = tuple.second;
      
      // The upper limit of "offset" is 26, because after offsetting 26 characters in any substring of "s", we'd not find any new substring of any length. 
      int offsetUpper = min(r, 26);
      for (int offset = 0; offset < offsetUpper; ++offset) {
        int lenUpper = max(r-offset, 0);
        if (lenUpper <= 0) continue; 
        for (int len = lenUpper; len >= 1; --len) {
          int disFromA = (int)(ch - 'a');
          int offSetFromA = (disFromA+offset)%26;
          char startingCh = 'a' + offSetFromA;
          if (true == cache[startingCh][len]) continue;
          //printf("Added {%c, %d}\n", startingCh, len);    
          cache[startingCh][len] = true;
          ++ans;
        }
      }
    }
  
    int findSubstringInWraproundString(string p) {
      int len = p.length(), consecutiveValidLength = 0;
      list<pair<char, int>> tuples;
      for (int i = 0; i < len; ++i) {
        if (0 == i) {
          consecutiveValidLength = 1;
          continue;
        }
        char expectingCh = p[i-1] + 1; 
        if (expectingCh > 'z') expectingCh = 'a';
        if (p[i] == expectingCh) {
          ++consecutiveValidLength;
        } else {
          pair<char, int> tuple = {p[i-consecutiveValidLength], consecutiveValidLength};
          //printf("@i:%d, settling consecutiveValidLength:%d, tuple {ch: %c, r:%d}\n", i, consecutiveValidLength, tuple.first, tuple.second);
          tuples.push_back(tuple);
          consecutiveValidLength = 1;
        }
      }
      
      // wrap up
      if (consecutiveValidLength > 0) {
        pair<char, int> tuple = {p[len-consecutiveValidLength], consecutiveValidLength};
        tuples.push_back(tuple);
        consecutiveValidLength = 0;
      }
      
      unordered_map<char, unordered_map<int, bool>> cache; // "cache[ch][r] == true" means that the substring in "s" that starts with character 'ch' and extends to length 'r' is already accounted
      int ans = 0;
      for (auto &tuple : tuples) {
        // printf("Counting tuple {ch: %c, r:%d}\n", tuple.first, tuple.second);
        count(cache, tuple, ans);
      }
      
      return ans;
    }
};
