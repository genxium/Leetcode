class Solution {
public:
    void incrementBy1(string& s, int &lastZPos) {
      if (lastZPos+1 < s.length()) {
        s[lastZPos+1] += 1;
        if (s[lastZPos+1] == 'z') ++lastZPos;
      }
    }
  
    string getSmallestString(int n, int k) {
      string s = "";
      int lastZPos = -1;
      for (int i = 0; i < n; ++i) s.push_back('a');
      k -= n;
      while (k > 0) {
        incrementBy1(s, lastZPos);
        --k;
      }
      reverse(s.begin(), s.end());
      return s;
    }
};
