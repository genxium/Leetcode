class Solution {
public:
    int minInsertions(string s) {
      /*
      test case #1
      ")))))))"
      
      test case #2
      "(()))(()))()())))"
      */
      int unmatchedRightHalfCnt = 0;
      int extraRightHalvesToAdd = 0;

      stack<pair<char, int>> stk;
      for (auto &ch : s) {
        if (ch == '(') {
          if (!stk.empty() && 1 == stk.top().second) {
            ++extraRightHalvesToAdd;
            stk.pop();        
          } else {
            stk.push({'(', 2});    
          }
        } else {
          if (stk.empty()) {
            ++unmatchedRightHalfCnt;
          } else {
            --stk.top().second;
            if (0 == stk.top().second) {
              stk.pop();
            }
          }
        }
      }
      printf("unmatchedRightHalfCnt:%d, stk.size():%d, extraRightHalvesToAdd:%d\n", unmatchedRightHalfCnt, stk.size(), extraRightHalvesToAdd);
      
      while (!stk.empty()) {
        extraRightHalvesToAdd += stk.top().second;
        stk.pop();
      }
      
      int extraLeftHalvesToAdd = (unmatchedRightHalfCnt+1)/2; // works for either odd or even "unmatchedRightHalfCnt"
      if ((unmatchedRightHalfCnt & 1) > 0) {
        extraRightHalvesToAdd += 1;
      }
      
      return (extraLeftHalvesToAdd+extraRightHalvesToAdd);
    }
};
