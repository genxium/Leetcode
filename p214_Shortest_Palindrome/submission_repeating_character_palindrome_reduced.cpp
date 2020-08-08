class Solution {
  public:
    int longestPalindromeFromBeginning(string &s) {
      int slen = s.length();
      if (slen <= 1) return 1;

      // init
      vector< vector<int> > palindromeLenCount; // "palindromeLenCount[i][j] == n" means that "the j-th palindrome ended at s[i] has length n" 
      palindromeLenCount.push_back({1}); // A single character itself is of course a palindrome.

      int toRet = 1;

      // loop
      for (int i = 1; i < slen; ++i) {
        vector<int> currentPalindromeLenCount = {1}; // A single character itself is of course a palindrome.
        vector<int> prevPalindromeLenCount = palindromeLenCount[i - 1];
        /*
           Re-purpose "currentPalindromeLenCount[0]" to contain the length of the "repeating character palindrome ended at each s[i]", which must be a palindrome by itself.

           Without such "re-purposing", besides the palindromes of pattern "aaaa....aaaa"(all the same), the following iteration of "j" is only of O(log2(i)) time-complexity. 

           Therefore I try to tackle the edge-case of "repeating character palindrome ended at each s[i]".
         */
        for (int j = 0; j < prevPalindromeLenCount.size(); ++j) {
          int prevPalindromeLen = prevPalindromeLenCount[j]; // could be ODD or EVEN
          int leftIdxCandidateOfCurrentPalindrome = -1, newPalindromeLen = 0;
          do {
            if (0 < (prevPalindromeLen & 1)) {
              // ODD
              if (0 == j) {
                if (1 < prevPalindromeLen) {
                  // A "repeating character palindrome", otherwise "prevPalindromeLenCount[0]" should be 1.
                  if (s[i] == s[i-1]) {
                    currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                    leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                    newPalindromeLen = currentPalindromeLenCount[0];
                    break;             
                  }
                } else {
                  // 1 == prevPalindromeLen
                  if (s[i] == s[i-1]) {
                    currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                    leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                    newPalindromeLen = currentPalindromeLenCount[0];
                    break;                  
                  }
                }
              }
            } else {
              // EVEN
              if (0 == j) {
                // A "repeating character palindrome", otherwise "prevPalindromeLenCount[0]" should be 1 which is ODD.
                if (s[i] == s[i-1]) {
                  /* 
                     Joins the "repeating character palindrome", it's IMPOSSIBLE that 
                     ```
                     s[(i-1)-(prevPalindromeLen - 1)-1] == s[i]
                     ```
                     , otherwise "prevPalindromeLen" should be ODD instead and bigger.
                   */
                  currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                  leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                  newPalindromeLen = currentPalindromeLenCount[0];
                  break;
                }
              }
            }

            int leftIdxOfPrevPalindrome = (i - 1) - (prevPalindromeLen - 1); 
            leftIdxCandidateOfCurrentPalindrome = (leftIdxOfPrevPalindrome - 1); 
            if (0 > leftIdxCandidateOfCurrentPalindrome) break;
            char leftCandidate = s[leftIdxCandidateOfCurrentPalindrome];
            if (leftCandidate != s[i]) break;
            newPalindromeLen = (prevPalindromeLen + 2);
            currentPalindromeLenCount.push_back(newPalindromeLen); 
          } while (false);

          if (0 == newPalindromeLen) {
            continue;
          }

          if (0 == leftIdxCandidateOfCurrentPalindrome) {
            if (newPalindromeLen > toRet) {
              toRet = newPalindromeLen;
            }
          }
        }  
        palindromeLenCount.push_back(currentPalindromeLenCount);
      }

      return toRet;
    }

    string shortestPalindrome(string s) {
      int toPrependStart = longestPalindromeFromBeginning(s);
      string prepend = "";
      for (int i = toPrependStart; i < s.length(); ++i) {
        prepend.push_back(s[i]);
      }
      reverse(prepend.begin(), prepend.end());
      string ans = prepend + s;
      return ans;
    }
};
