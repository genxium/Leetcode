class Solution {
public:
    string parseFactor(int& idx, string &s) {
      string toRet = "";
      if (idx == s.length()) return toRet;
            
      if (isalpha(s[idx])) {
        while (idx < s.length() && isalpha(s[idx])) {
          toRet.push_back(s[idx]);
          ++idx;  
        }
        return toRet;
      }
      
      // s[idx] is definitely numeric
      int count = 0;
      while (idx < s.length() && isdigit(s[idx])) {
        count *= 10;
        count += (int)(s[idx]-'0');
        ++idx;
      }
      
      // s[idx] is definitely '[' now
      string child = "";
      
      ++idx;
      while (idx < s.length() && ']' != s[idx]) {
        child.append(parseFactor(idx, s));
      }
      // s[idx] is definitely ']' now
      ++idx;
      
      for (int i = 0; i < count; ++i) {
        toRet.append(child);
      }
      
      return toRet;
    }
  
    string decodeString(string s) {
      /*
      factor := |alpha
             := |alpha concat alpha
             := |alpha concat alpha concat alpha ...
             := |number[factor]
             := |number[factor concat factor]
             := |number[factor concat factor concat factor ...]
          
      ans := |factor
             |factor concat factor
             |factor concat factor concat factor ...
      */
      int idx = 0;
      string ans = "";
      while (idx < s.length()) {
        ans.append(parseFactor(idx, s));  
      }
      return ans;
    }
};
