#define LL  int64_t
#define ULL uint64_t
#define MOD 16u
#define MAXDIGITCOUNT 8
/*
test case#1
0

test case#2
-100000

test case#3
-2147483648
*/
class Solution {
public:
    char toHexChar(int digit) {
      if (digit < 10) {
        return '0' + digit;
      }
      return 'a' + (digit-10);
    }
  
    string toHex(int num) {
      vector<ULL> tmp(MAXDIGITCOUNT, 0);
      
      ULL absnum = (ULL)abs((LL)num);
      int idx = MAXDIGITCOUNT-1;
      while (absnum > 0u) {
        int digit = absnum%MOD;
        tmp[idx] = digit;
        absnum /= MOD;
        --idx;
      }
      if (num < 0) {
        // 16's complement
        for (int i = 0; i < MAXDIGITCOUNT; ++i) {
          tmp[i] = MOD-1-tmp[i];
        }
        idx = MAXDIGITCOUNT-1;
        int sumOverflow = 1;
        while (sumOverflow > 0) {
          tmp[idx] += sumOverflow;
          if (tmp[idx] >= MOD) {
            tmp[idx] -= MOD;
            sumOverflow = 1;
          } else {
            sumOverflow = 0;
          }
          --idx;
        }
      }
      
      string ans = "";
      for (int i = 0; i < MAXDIGITCOUNT; ++i) {
        if (ans.empty() && 0 == tmp[i]) continue;
        ans.push_back(toHexChar(tmp[i]));
      }
      if (ans.empty()) {
        ans.push_back('0');
      }
      return ans;
    }
};
