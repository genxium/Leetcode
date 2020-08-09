class Solution {
public:
    int minInsertions(string s) {
      /*
      test case #1
      ")))))))"
      
      test case #2
      "(()))(()))()())))"
      
      test case #3
      ")())))))()))"
      
      test case #4
      "))(()()))()))))))()())()(())()))))()())(()())))()("
      */
        
      int orphanedRightHalvesCnt = 0;
      int extraLeftHalvesToAdd = 0;
          
      int extraRightHalvesToAdd = 0;
      stack<pair<char, int>> stk;
      for (auto &ch : s) {
        if (ch == '(') {
          if (!stk.empty() && 1 == stk.top().second) {
            ++extraRightHalvesToAdd;
            stk.pop();        
          }
          stk.push({'(', 2}); 
          // It's a MUST to update "extraLeftHalvesToAdd" upon every incoming '(', for cases where 2 ODD "orphanedRightHalvesCnt" couldn't add up to contribute to "extraLeftHalvesToAdd"!
          extraLeftHalvesToAdd += (orphanedRightHalvesCnt + 1)/2; // works for either odd or even "orphanedRightHalvesCnt"
          if ((orphanedRightHalvesCnt & 1) > 0) {
            extraRightHalvesToAdd += 1;
          }
          orphanedRightHalvesCnt = 0;
        } else {
          if (stk.empty()) {
            ++orphanedRightHalvesCnt;
          } else {
            --stk.top().second;
            if (0 == stk.top().second) {
              stk.pop();
            }
          }
        }
      }
      
      // wrap up
      extraLeftHalvesToAdd += (orphanedRightHalvesCnt + 1)/2;
      if ((orphanedRightHalvesCnt & 1) > 0) {
        extraRightHalvesToAdd += 1;
      }
      while (!stk.empty()) {
        extraRightHalvesToAdd += stk.top().second;
        stk.pop();
      }
      
      return (extraLeftHalvesToAdd+extraRightHalvesToAdd);
    }
};
