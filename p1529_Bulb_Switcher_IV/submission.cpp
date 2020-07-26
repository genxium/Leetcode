class Solution {
public:
    int minFlips(string target) {
      /*
      An operation cycle is as follows.

      current: ....00000000[MATCHED]
      target:  ....01111111...

      flip#1
      current: ....01111111[!MATCHED]
                    ^
      target:  ....01111111...

      flip#2
      current: ....01111111[MATCHED]
                           ^ 
      target:  ....01111111...
      */
      int continuousOneSegs = 0;
      bool reverseStartsWithZero = (*target.rbegin() == '0' ? true : false);
      char prev = '0';
      for (auto rit = target.rbegin(); rit != target.rend(); ++rit) {
        char c = *rit;
        if (c == '0') {
          if (prev == '1') {
            ++continuousOneSegs; 
          }
        }
        prev = c;
      }
      // wrap up
      if (prev == '1') {
        ++continuousOneSegs; 
      }
      
      int ans = (continuousOneSegs << 1);
      if (reverseStartsWithZero == false) ans -= 1;
      if (ans < 0) ans = 0;
      return ans;
    }
};
