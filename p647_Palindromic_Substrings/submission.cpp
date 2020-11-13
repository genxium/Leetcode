/*
test case#1
"aaaabbbbabaaaababbbababa"

test case#2
"a"
*/
class Solution {
public:
    int longestPalindromeFromBeginning(string s, int &count) {
      /*
      Blindly copied from https://github.com/genxium/Leetcode/tree/master/p5_Longest_Palindromic_Substring.
      */
      int slen = s.length();
      if (slen <= 1) {
        count = 1;
        return 1;
      }

      string ss = "#"; // definitely of length (2*slen + 1) and of which any “palindromic substring” must be of ODD length
      for (int i = 0; i < slen; ++i) {
        ss.push_back(s[i]);
        ss.push_back('#');
      }
      int sslen = (slen << 1) + 1;

      // init
      vector< vector<int> > palindromeLenCount; // "palindromeLenCount[i][j] == n" means that "the j-th palindrome ended at ss[i] has length n" 
      palindromeLenCount.push_back({1}); // A single character itself is of course a palindrome.

      int toRet = 1;

      // loop
      for (int i = 1; i < sslen; ++i) {
        vector<int> currentPalindromeLenCount = {1}; // A single character itself is of course a palindrome.
        vector<int> prevPalindromeLenCount = palindromeLenCount[i - 1];
        for (int j = 0; j < prevPalindromeLenCount.size(); ++j) {
          int prevPalindromeLen = prevPalindromeLenCount[j]; // must be ODD
          int leftIdxOfPrevPalindrome = (i - 1) - (prevPalindromeLen - 1); 

          int leftIdxCandidateOfCurrentPalindrome = (leftIdxOfPrevPalindrome - 1); 
          if (leftIdxCandidateOfCurrentPalindrome < 0) continue;
          char leftCandidate = ss[leftIdxCandidateOfCurrentPalindrome];
          if (leftCandidate != ss[i]) continue;
          int newPalindromeLen = (prevPalindromeLen + 2);
          currentPalindromeLenCount.push_back(newPalindromeLen);

          if (0 == leftIdxCandidateOfCurrentPalindrome) {
            if (newPalindromeLen > toRet) {
              toRet = newPalindromeLen;
            }
          }
        }  
        palindromeLenCount.push_back(currentPalindromeLenCount);
        if (ss[i] != '#') {
          count += currentPalindromeLenCount.size();
        }
      }

      toRet >>= 1; // removes the '#'s
      return toRet;
    }

    int countSubstrings(string s) {
      int ans = 0;
      longestPalindromeFromBeginning(s, ans);
      return ans;
    }
};
