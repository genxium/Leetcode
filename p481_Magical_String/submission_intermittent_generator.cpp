int const S = 0;
int const SS = 1;

class Solution {
private: 
    char itoa(int x) {
      return '0'+x;
    }
  
    int atoi(char c) {
      return c-'0';
    }
  
public:
    int magicalString(int n) {
      if (0 == n) return 0;
      /*
      The magical string "s" is unique and determined in an intermittent manner.
      */
      int iS = 0, iSS = -1;
      int inspectingIndex = S;
      string s = "1", ss = "";
      while (iS < n && iSS < n) {
        switch (inspectingIndex) {
          case S: {
            char const cS = s[iS];
            int count = atoi(cS);
            ss.push_back(itoa(count)); ++iSS;
            inspectingIndex = SS;
            break;
          }
          case SS: {
            char const cSS = ss[iSS];
            int count_t = atoi(cSS);
            while (count_t > 1) {
              s.push_back(s.back());
              --count_t;
            }
            char nextCS = itoa(3-atoi(s.back())); 
            s.push_back(nextCS); ++iS; // complete "count_t" in "s"
            inspectingIndex = S;
            break;
          }
        }
      }
      
      // printf("iS == %d,\ts\t== %s\n", iS, s.c_str());
      // printf("iSS == %d,\tss\t== %s\n", iSS, ss.c_str());
      
      int ans = 0, i = 0;
      for (auto &c : s) {
        if (c == '1') ++ans;
        ++i;
        if (i == n) break;
      }
      return ans;
    }
};
