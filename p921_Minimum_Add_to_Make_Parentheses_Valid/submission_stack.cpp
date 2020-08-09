class Solution {
public:
    int minAddToMakeValid(string S) {
      /*
      test case #1
      "()))((()())((((())))))))))))))))()()()()))(()())))))))))))))"
      */
      int orphanedRightHalvesCnt = 0, l = S.length();
      stack<char> stk;
    
      for (int i = 0; i < l; ++i) {
        if (S[i] == '(') {
          stk.push(S[i]);
        } else {
          if (!stk.empty()) {
            stk.pop();
          } else {
            ++orphanedRightHalvesCnt;
          }
        }
      }
      return orphanedRightHalvesCnt + stk.size();
    }
};
