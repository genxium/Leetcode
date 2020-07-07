class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
      unordered_map<string, int> counter;
      int n = s.length();
      vector<string> ans;
      if (n < 10) return ans;
      int l = 0, r = 10;
      // init
      string subs = s.substr(l, r-l);
      while (r <= n) {
        if (counter.find(subs) == counter.end()) {
          counter[subs] = 1;
        } else {
          ++counter[subs];
        }
        ++l; ++r;
        if (r == n+1) break;
        subs.erase(0, 1); // This is an O(1) time-complexity operation, due to the constant length constraint of "subs".
        subs.push_back(s[r-1]);
      }
      
      for (auto it = counter.begin(); it != counter.end(); ++it) {
        if (it->second < 2) continue;
        ans.push_back(it->first);
      }
      return ans;
    }
};
